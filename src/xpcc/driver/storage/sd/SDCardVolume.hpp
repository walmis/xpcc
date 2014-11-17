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
	write_sem_taken(false),	reading(false), writing(false)
{}

private:
	DSTATUS doInitialize () override {
		XPCC_LOG_DEBUG .printf("%s()\n", __FUNCTION__);

		return RES_OK;
	}

	DSTATUS doGetStatus () override {
		//XPCC_LOG_DEBUG .printf("%s()\n", __FUNCTION__);
		return RES_OK;
	}

	void handleTick() {
		if(read_sem_taken && !reading) {
			if((xpcc::Clock::now() - last_op) > 5) {
				//XPCC_LOG_DEBUG .printf("release sem\n");
				last_block = 0;
				this->readStop();
				this->semaphore()->give();
				read_sem_taken = false;
			}
		}

		if(write_sem_taken && !writing) {
			if((xpcc::Clock::now() - last_op) > 5) {
				//XPCC_LOG_DEBUG .printf("release wr sem\n");
				last_block = 0;
				this->writeStop();
				this->semaphore()->give();
				write_sem_taken = false;
			}
		}
	}

	xpcc::fat::Result
	doRead(uint8_t *buffer, int32_t sectorNumber, uint32_t sectorCount) override {
		//XPCC_LOG_DEBUG .printf("%s(%d, %d)\n", __FUNCTION__, sectorNumber, sectorCount);

		if(!read_sem_taken) {
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
			//XPCC_LOG_DEBUG .printf("--start %d\n", sectorNumber);
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
			this->writeStart(sectorNumber, sectorCount);
		}

		while(sectorCount) {
			this->writeData(buffer);
			sectorCount--;
			buffer+=512;
		}

		last_block = sectorNumber + sectorCount-1;

		last_op = xpcc::Clock::now();

		writing = false;

		return RES_OK;
	}

	xpcc::fat::Result
	doIoctl(uint8_t command, uint32_t *buffer) override {
		XPCC_LOG_DEBUG .printf("%s(%d)\n", __FUNCTION__, command);

		if(command == GET_SECTOR_COUNT) {
			*buffer = this->_sectors;
			return RES_OK;
		}

		return RES_OK;
	}

private:
	uint32_t last_block;
	xpcc::Timestamp last_op;

	bool read_sem_taken; //semaphore taken by us?
	bool write_sem_taken; //semaphore taken by us?

	bool reading;
	bool writing;
};

#endif /* SDIO_HPP_ */
