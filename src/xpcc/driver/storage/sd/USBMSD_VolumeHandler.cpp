/*
 * USBMSD_VolumeHandler.cpp
 *
 *  Created on: Dec 4, 2014
 *      Author: walmis
 */

#include "USBMSD_VolumeHandler.hpp"

namespace xpcc {

void USBMSD_VolumeHandler::run() {
	if (requestedBlock != -1) {
		bool res = true;

		switch (opType) {
		case READ:

			if (!bufferedBlocksRemaining) {
				uint32_t cnt = std::min((uint32_t)bufferSize/512, blocksLeft ? blocksLeft : 1);
				//XPCC_LOG_DEBUG .printf("r1 %d %d\n", requestedBlock, cnt);
				res = volume.doRead(buffer, requestedBlock, cnt) == RES_OK;
				bufferedBlocksRemaining = cnt;
				bufPtr = buffer;
			}

			//XPCC_LOG_DEBUG .printf("s %d\n", requestedBlock);
			if(res) {
				memcpy(dataptr, bufPtr, 512);
				bufPtr+=512;
				bufferedBlocksRemaining--;
				haveBlock = -1;
			}

			if (blocksLeft != 0) {
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
//				XPCC_LOG_DEBUG.printf("MSD::write_begins r:%d c:%d\n",
//						requestedBlock, totalBlocks);
				volume.doIoctl(CTRL_ERASE_COUNT, &totalBlocks);

				firstBlock = false;
			}

			uint32_t block = requestedBlock;
			memcpy(buffer, dataptr, 512);
			requestedBlock = -1;
			//size_t left = blocksLeft;
			disk_write_finalize(true);

			//XPCC_LOG_DEBUG .printf("wr %d\n", block);
			res = volume.doWrite(buffer, block, 1) == RES_OK;

			break;
		}
	}

	//read some blocks in advance
	if (!bufferedBlocksRemaining && readBlock != -1) {
		uint32_t cnt = std::min((uint32_t)bufferSize/512, blocksLeft);
		if(cnt) {
			//XPCC_LOG_DEBUG .printf("r2 %d %d\n", readBlock, cnt);
			if (volume.doRead(buffer, readBlock, cnt) == RES_OK) {
				bufPtr = buffer;
				bufferedBlocksRemaining = cnt;
				readBlock = -1;
			}
		} else {
			readBlock = -1;
		}
	}

}

void USBMSD_VolumeHandler::runBlocking() {
	event.wait();
	run();
}
//called from usb interrupt//
void USBMSD_VolumeHandler::transfer_begins(TransferType type,
		uint32_t startBlock, int numBlocks) {
	//XPCC_LOG_DEBUG .printf("tr t:%d start:%d num:%d\n", (uint32_t)type, startBlock, numBlocks);
	bufferedBlocksRemaining = 0;
	bufPtr = buffer;

	requestedBlock = -1;
	opType = type;
	totalBlocks = numBlocks;
	firstBlock = true;
	event.signal();
}
//called from usb interrupt//
int USBMSD_VolumeHandler::disk_read_start(uint8_t * data, uint32_t block,
		uint32_t blocksLeft) {
	requestedBlock = block;
	dataptr = data;
	this->blocksLeft = blocksLeft;
	event.signal();
	return 0;
}
//called from usb interrupt//
int USBMSD_VolumeHandler::disk_write_start(const uint8_t * data, uint32_t block,
		uint32_t blocksLeft) {

	//XPCC_LOG_DEBUG .printf("wr start block:%d left:%d\n", block, blocksLeft);

	requestedBlock = block;
	dataptr = (uint8_t*)data;
	this->blocksLeft = blocksLeft;
	event.signal();
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

	if((status & STA_PROTECT))
	return DISK_OK | WRITE_PROTECT;

	return DISK_OK;

}

}
