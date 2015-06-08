/*
 * SDIO.hpp
 *
 *  Created on: Oct 30, 2013
 *      Author: walmis
 */

#ifndef SDIO_HPP_
#define SDIO_HPP_

#include <xpcc/processing.hpp>
#include <fatfs/diskio.h>
#include <fatfs/ff.h>
#include <xpcc/driver/storage/fat.hpp>
#include <xpcc/driver/storage/sd/SDCardSPI.h>

#define INVALID 0xFFFFFFFE

template <typename Card>
class SDCardVolume : public Card, public xpcc::fat::PhysicalVolume,
	xpcc::TickerTask {

public:
	SDCardVolume() : last_block(INVALID), read_sem_taken(false),
	write_sem_taken(false),	reading(false), writing(false),
	eraseCount(0), wrRemaining(0)
{}

	DSTATUS doInitialize () override {
		//XPCC_LOG_DEBUG .printf("%s()\n", __FUNCTION__);

		if(!this->init())
			return STA_NODISK;

		return 0;
	}

	DSTATUS doGetStatus () override {
		//XPCC_LOG_DEBUG .printf("%s()\n", __FUNCTION__);
		if(!this->isInitialized()) {
			return STA_NODISK;
		}

		return 0;
	}

	void stopRead() {
		if(read_sem_taken) {
			last_block = INVALID;
			this->readStop();
			this->semaphore()->give();
			read_sem_taken = false;
		}
	}

	void stopWrite() {
		if(write_sem_taken) {
			if(wrRemaining != 0) {
				XPCC_LOG_WARNING .printf("erase count != 0 before stop\n");
			}
			last_block = INVALID;
			this->writeStop();

			this->semaphore()->give();
			write_sem_taken = false;
		}
	}

	void handleTick() {
		if(read_sem_taken && !reading) {
			if((xpcc::Clock::now() - last_op) > SD_READ_TIMEOUT) {
				stopRead();
			}
		}

		if(write_sem_taken && !writing && eraseCount == 0) {
			if((xpcc::Clock::now() - last_op) > SD_WRITE_TIMEOUT) {
				//XPCC_LOG_DEBUG .printf("=release wr sem\n");
				stopWrite();
			}
		}
	}

	xpcc::fat::Result
	doRead(uint8_t *buffer, int32_t sectorNumber, uint32_t sectorCount) override {
		if(!operation.take(600)) {
			XPCC_LOG_ERROR .printf("RD operation.take() timeout\n");
			return RES_ERROR;
		}

		if(!read_sem_taken) {
			if(write_sem_taken) {
				stopWrite(); //stop write operation, and begin read
			}
			if(!this->semaphore()->take(200)) {
				XPCC_LOG_ERROR .printf("Card sem.take() timeout\n");
				operation.give();
				return RES_ERROR;
			}
			read_sem_taken = true;
			last_block = INVALID;
		}

		reading = true;
		//XPCC_LOG_DEBUG .printf("doRead(%d, %d) lb:%d\n", sectorNumber, sectorCount, last_block);

		if(last_block+1 != sectorNumber) {
			//XPCC_LOG_DEBUG .printf("last block %d\n", last_block);
			if(last_block != INVALID) {
				this->readStop();
			}
			this->readStart(sectorNumber);

			//XPCC_LOG_DEBUG .printf("read start %d\n", sectorNumber);
		}

		last_block = sectorNumber + sectorCount - 1;

		this->readData(buffer, 512*sectorCount);

		last_op = xpcc::Clock::now();

		reading = false;

		operation.give();
		return RES_OK;
	}

	xpcc::fat::Result
	doWrite(const uint8_t *buffer, int32_t sectorNumber, uint32_t sectorCount) override {
		if(!operation.take(600)) {
			XPCC_LOG_ERROR .printf("WR operation.take() timeout\n");
			return RES_ERROR;
		}

		if(!write_sem_taken) {
			if(read_sem_taken) {
				stopRead(); //stop read operation, and begin write
			}
			if(!this->semaphore()->take(200)) {
				XPCC_LOG_ERROR .printf("Card sem.take() timeout\n");
				operation.give();
				return RES_ERROR;
			}
			write_sem_taken = true;
			last_block = INVALID;
			eraseCount = 0;
		}

		//XPCC_LOG_DEBUG .printf("%s(%d, %d)\n", __FUNCTION__, sectorNumber, sectorCount);
		writing = true;

		if(last_block+1 != sectorNumber) {
			if(last_block != INVALID) {
				this->writeStop();
			}
			this->writeStart(sectorNumber, eraseCount);
			wrRemaining = eraseCount;
			eraseCount = 0;
		}

		last_block = sectorNumber + sectorCount - 1;

		while(sectorCount) {
			this->writeData(buffer);
			if(wrRemaining) {
				wrRemaining--;
				if(!wrRemaining) {
					last_block = INVALID-1;
				}
			}
			sectorCount--;
			buffer+=512;
		}

		last_op = xpcc::Clock::now();

		writing = false;

		operation.give();
		return RES_OK;
	}

	xpcc::fat::Result
	doIoctl(uint8_t command, uint32_t *buffer) override {
		//XPCC_LOG_DEBUG .printf("%s(%d)\n", __FUNCTION__, command);

		switch(command) {
		case CTRL_SYNC:
			return RES_OK;

		case GET_SECTOR_COUNT:
			if(!this->isInitialized()) {
				*buffer = 0;
			} else {
				*buffer = this->getSectorCount();
			}
			return RES_OK;
		case GET_SECTOR_SIZE:
			*buffer = 512;
			return RES_OK;
		case CTRL_ERASE_COUNT:
			eraseCount = *buffer;
			return RES_OK;
		}

		return RES_ERROR;
	}

private:
	uint32_t last_block;
	xpcc::Timestamp last_op;

	xpcc::Semaphore operation;

	volatile bool read_sem_taken; //semaphore taken by us?
	volatile bool write_sem_taken; //semaphore taken by us?

	volatile bool reading;
	volatile bool writing;
	uint32_t eraseCount;
	uint32_t wrRemaining;
};


#undef INVALID
#endif /* SDIO_HPP_ */
