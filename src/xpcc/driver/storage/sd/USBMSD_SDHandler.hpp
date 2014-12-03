/*
 * USBMSD_SDHandler.hpp
 *
 *  Created on: Oct 31, 2013
 *      Author: walmis
 */

#ifndef USBMSD_SDHANDLER_HPP_
#define USBMSD_SDHANDLER_HPP_

#include <xpcc/architecture.hpp>
#include <xpcc/driver/storage/fat.hpp>
#include "SDCard.h"

namespace xpcc {

class USBMSD_VolumeHandler : public USBMSDHandler, TickerTask {
public:
	USBMSD_VolumeHandler(xpcc::fat::PhysicalVolume &vol,
			uint8_t bulkIn = EPBULK_IN, uint8_t bulkOut = EPBULK_OUT) :
                            USBMSDHandler(bulkIn, bulkOut), volume(vol) {
		haveBlock = -1;
		readBlock = -1;
		requestedBlock = -1;
		writeBusy = false;
	}


protected:
    volatile uint32_t requestedBlock;

    uint8_t buffer[512];
    uint8_t *dataptr;
    uint32_t blocksLeft;

    TransferType opType;
    uint32_t totalBlocks;
    volatile bool firstBlock;
    volatile int32_t haveBlock;
    volatile int32_t readBlock;
    volatile bool writeBusy;

    void handleTick() override {
    	if(readBlock != -1) {
			if(volume.doRead(buffer, readBlock, 1) == RES_OK) {
				haveBlock = readBlock;
			}
			readBlock = -1;
    	}

    	if(requestedBlock != -1) {
    		bool res;

    		switch(opType) {
    		case READ:
    			if(firstBlock) {
//    	    		if(!card->semaphore()->take_nonblocking()) {
//    	    			XPCC_LOG_DEBUG .printf("*");
//    	    			return;
//    	    		}
    				XPCC_LOG_DEBUG .printf("MSD::read_begins %d a:%d d:%d\n", opType, requestedBlock, totalBlocks);
    				//card->readStart(requestedBlock);
    				firstBlock = false;
    			}

    			if(haveBlock != requestedBlock) {
    				res = volume.doRead(dataptr, requestedBlock, 1) == RES_OK;

    			} else {
    				//XPCC_LOG_DEBUG.printf("+");
    				memcpy(dataptr, buffer, 512);
    				haveBlock = -1;
    				res = true;
    			}

    			if(blocksLeft == 0) {
    				//card->readStop();
    				//card->semaphore()->give();
    			} else {
    				//read in advance
    				readBlock = requestedBlock+1;
    			}

    			requestedBlock = -1;

				if(res) {
					disk_read_finalize(true);
				} else {
					disk_read_finalize(false);
				}

    			break;

    		case WRITE:
    			if(firstBlock) {
//    	    		if(!card->semaphore()->take_nonblocking()) {
//    	    			XPCC_LOG_DEBUG .printf("*");
//    	    			return;
//    	    		}
    				XPCC_LOG_DEBUG .printf("MSD::write_begins r:%d c:%d\n", requestedBlock, totalBlocks);

    				//card->writeStart(requestedBlock, totalBlocks);
    				firstBlock = false;
    			}

    			uint32_t block = requestedBlock;
    			memcpy(buffer, dataptr, 512);
    			requestedBlock = -1;
    			//size_t left = blocksLeft;

    			disk_write_finalize(true);

    			res = volume.doWrite(buffer, block, 1) == RES_OK;

//    			if(left == 0) {
//    				card->writeStop();
//    				card->semaphore()->give();
//    			}
    			break;
    		}
    	}

    }

    void transfer_begins(TransferType type, uint32_t startBlock, int numBlocks) override{
		requestedBlock = -1;
		opType = type;
		totalBlocks = numBlocks;
		firstBlock = true;
    }

    int disk_read_start(uint8_t * data, uint32_t block, uint32_t blocksLeft) override {
    	requestedBlock = block;
    	dataptr = data;
    	this->blocksLeft = blocksLeft;

		return 0;
    }

    int disk_write_start(const uint8_t * data, uint32_t block, uint32_t blocksLeft) override {
    	requestedBlock = block;
    	dataptr = (uint8_t*)data;
    	this->blocksLeft = blocksLeft;

        return 0;
    }

    int disk_initialize() override {
            XPCC_LOG_DEBUG .printf("disk initialize()\n");

            return DISK_OK;
    }

    uint32_t disk_sectors() override {
        uint32_t res;
    	if(volume.doIoctl(GET_SECTOR_COUNT, &res) == RES_OK) {
    		return res;
    	}

    	return 0;
    }

    uint16_t disk_sector_size() override {
        uint32_t res;
    	if(volume.doIoctl(GET_SECTOR_SIZE, &res) == RES_OK) {
    		return res;
    	}
    	XPCC_LOG_ERROR .printf("Unable to get sector size\n");
    	return 512;
    }

    int disk_status() override {
    	//return NO_DISK;
    	//return DISK_OK | WRITE_PROTECT;
    	DSTATUS status = volume.doGetStatus();

    	if(status & (STA_NODISK|STA_NOINIT))
    		return NO_DISK;

    	if(status & (STA_PROTECT))
    		return DISK_OK | WRITE_PROTECT;

        return DISK_OK;

    }

protected:
	xpcc::fat::PhysicalVolume& volume;
};

}


#endif /* USBMSD_SDHANDLER_HPP_ */
