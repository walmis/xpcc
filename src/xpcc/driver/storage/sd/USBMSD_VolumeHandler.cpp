/*
 * USBMSD_VolumeHandler.cpp
 *
 *  Created on: Dec 4, 2014
 *      Author: walmis
 */

#include "USBMSD_VolumeHandler.hpp"
namespace xpcc {

void USBMSD_VolumeHandler::handleTick() {
	if (readBlock != -1) {
		if (volume.doRead(buffer, readBlock, 1) == RES_OK) {
			haveBlock = readBlock;
		}
		readBlock = -1;
	}

	if (requestedBlock != -1) {
		bool res;

		switch (opType) {
		case READ:
			if (firstBlock) {
//    	    		if(!card->semaphore()->take_nonblocking()) {
//    	    			XPCC_LOG_DEBUG .printf("*");
//    	    			return;
//    	    		}
				XPCC_LOG_DEBUG.printf("MSD::read_begins %d a:%d d:%d\n", opType,
						requestedBlock, totalBlocks);
				//card->readStart(requestedBlock);
				firstBlock = false;
			}

			if (haveBlock != requestedBlock) {
				res = volume.doRead(dataptr, requestedBlock, 1) == RES_OK;

			} else {
				//XPCC_LOG_DEBUG.printf("+");
				memcpy(dataptr, buffer, 512);
				haveBlock = -1;
				res = true;
			}

			if (blocksLeft == 0) {
				//card->readStop();
				//card->semaphore()->give();
			} else {
				//read in advance
				readBlock = requestedBlock + 1;
			}

			requestedBlock = -1;

			if (res) {
				disk_read_finalize(true);
			} else {
				disk_read_finalize(false);
			}

			break;

		case WRITE:
			if (firstBlock) {
//    	    		if(!card->semaphore()->take_nonblocking()) {
//    	    			XPCC_LOG_DEBUG .printf("*");
//    	    			return;
//    	    		}
				XPCC_LOG_DEBUG.printf("MSD::write_begins r:%d c:%d\n",
						requestedBlock, totalBlocks);
				volume.doIoctl(CTRL_ERASE_COUNT, &totalBlocks);

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

void USBMSD_VolumeHandler::transfer_begins(TransferType type,
		uint32_t startBlock, int numBlocks) {
	requestedBlock = -1;
	opType = type;
	totalBlocks = numBlocks;
	firstBlock = true;
}

int USBMSD_VolumeHandler::disk_read_start(uint8_t * data, uint32_t block,
		uint32_t blocksLeft) {
	requestedBlock = block;
	dataptr = data;
	this->blocksLeft = blocksLeft;

	return 0;
}

int USBMSD_VolumeHandler::disk_write_start(const uint8_t * data, uint32_t block,
		uint32_t blocksLeft) {
	requestedBlock = block;
	dataptr = (uint8_t*)data;
	this->blocksLeft = blocksLeft;

	return 0;
}

int USBMSD_VolumeHandler::disk_initialize() {
	XPCC_LOG_DEBUG .printf("disk initialize()\n");

	return DISK_OK;
}

uint32_t USBMSD_VolumeHandler::disk_sectors() {
	uint32_t res;
	if(volume.doIoctl(GET_SECTOR_COUNT, &res) == RES_OK) {
		return res;
	}
	XPCC_LOG_ERROR .printf("Unable to get sector count\n");
	return 0;
}

uint16_t USBMSD_VolumeHandler::disk_sector_size() {
	uint32_t res;
	if(volume.doIoctl(GET_SECTOR_SIZE, &res) == RES_OK) {
		return res;
	}
	XPCC_LOG_ERROR .printf("Unable to get sector size\n");
	return 512;
}

int USBMSD_VolumeHandler::disk_status() {
	//return DISK_OK | WRITE_PROTECT;
	DSTATUS status = volume.doGetStatus();

	if(status & (STA_NODISK|STA_NOINIT))
	return NO_DISK;

	if(status & (STA_PROTECT))
	return DISK_OK | WRITE_PROTECT;

	return DISK_OK;

}

}
