/*
 * SDIO.hpp
 *
 *  Created on: Oct 30, 2013
 *      Author: walmis
 */

#ifndef SDIO_HPP_
#define SDIO_HPP_

#include <xpcc/processing.hpp>
#include "SDCard.h"
#include <fatfs/diskio.h>
#include <fatfs/ff.h>
#include <xpcc/driver/storage/fat.hpp>


template <typename Spi, typename Cs>
class SDCardVolume : public SDCard<Spi,Cs>,	public xpcc::fat::PhysicalVolume,
	xpcc::TickerTask {

public:
	SDCardVolume() : last_block(0), read_sem_taken(false),
	write_sem_taken(false),	reading(false), writing(false),
	eraseCount(0)
{}

private:
	DSTATUS doInitialize () override {
		XPCC_LOG_DEBUG .printf("%s()\n", __FUNCTION__);

		if(!this->initialise())
			return STA_NODISK;

		return 0;
	}

	DSTATUS doGetStatus () override {
		XPCC_LOG_DEBUG .printf("%s()\n", __FUNCTION__);
		if(!this->initialized) {
			return STA_NOINIT;
		}

		return 0;
	}

	void stopRead() {
		last_block = 0;
		this->readStop();
		this->semaphore()->give();
		read_sem_taken = false;
		XPCC_LOG_DEBUG .printf("read stop\n");
	}

	void stopWrite() {
		last_block = 0;
		this->writeStop();
		this->semaphore()->give();
		write_sem_taken = false;
		XPCC_LOG_DEBUG .printf("write stop\n");
	}

	void handleTick() {
		if(read_sem_taken && !reading) {
			if((xpcc::Clock::now() - last_op) > 5) {
				//XPCC_LOG_DEBUG .printf("release sem\n");
				stopRead();
			}
		}

		if(write_sem_taken && !writing) {
			if((xpcc::Clock::now() - last_op) > 5) {
				//XPCC_LOG_DEBUG .printf("release wr sem\n");
				stopWrite();
			}
		}
	}

	xpcc::fat::Result
	doRead(uint8_t *buffer, int32_t sectorNumber, uint32_t sectorCount) override {
		//XPCC_LOG_DEBUG .printf("%s(%d, %d)\n", __FUNCTION__, sectorNumber, sectorCount);

		if(!read_sem_taken) {
			if(write_sem_taken) {
				while(writing) {
					//wait for write to complete
					yield();
				}
				stopWrite(); //stop write operation, and begin read
			}
			if(!this->semaphore()->take(200)) {
				XPCC_LOG_ERROR .printf("sd timeout\n");
				return RES_ERROR;
			}
			read_sem_taken = true;
			last_block = 0;
		}

		reading = true;

		if(last_block+1 != sectorNumber) {
			this->readStop();
			this->readStart(sectorNumber);
			XPCC_LOG_DEBUG .printf("read start %d\n", sectorNumber);
		}

		last_block = sectorNumber + sectorCount - 1;

		for(int i = 0; i < sectorCount; i++) {
			this->readData(buffer, 512);
			buffer+=512;
		}

		last_op = xpcc::Clock::now();

		reading = false;

		return RES_OK;
	}

	xpcc::fat::Result
	doWrite(const uint8_t *buffer, int32_t sectorNumber, uint32_t sectorCount) override {
		XPCC_LOG_DEBUG .printf("%s(%d, %d)\n", __FUNCTION__, sectorNumber, sectorCount);
		if(!write_sem_taken) {
			if(read_sem_taken) {
				while(reading) {
					//wait for read to complete
					yield();
				}
				stopRead(); //stop read operation, and begin write
			}
			if(!this->semaphore()->take(200)) {
				XPCC_LOG_ERROR .printf("sd timeout\n");
				return RES_ERROR;
			}
			write_sem_taken = true;
			last_block = 0;
		}

		writing = true;

		if(last_block+1 != sectorNumber) {
			if(last_block != 0) {
				this->writeStop();
			}
			this->writeStart(sectorNumber, eraseCount);
			eraseCount = 0;
			XPCC_LOG_DEBUG .printf("write start count:%d\n", eraseCount);
		}

		last_block = sectorNumber + sectorCount - 1;

		while(sectorCount) {
			this->writeData(buffer);
			sectorCount--;
			buffer+=512;
		}

		last_op = xpcc::Clock::now();

		writing = false;

		return RES_OK;
	}

	xpcc::fat::Result
	doIoctl(uint8_t command, uint32_t *buffer) override {
		XPCC_LOG_DEBUG .printf("%s(%d)\n", __FUNCTION__, command);

		switch(command) {
		case GET_SECTOR_COUNT:
			if(!this->initialized) {
				*buffer = 0;
			} else {
				*buffer = this->_sectors;
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

	volatile bool read_sem_taken; //semaphore taken by us?
	volatile bool write_sem_taken; //semaphore taken by us?

	volatile bool reading;
	volatile bool writing;
	uint32_t eraseCount;
};

#endif /* SDIO_HPP_ */
