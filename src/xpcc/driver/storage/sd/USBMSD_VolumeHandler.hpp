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
#include <xpcc/driver/connectivity/usb/USBDevice/USBMSD/USBMSDHandler.h>

namespace xpcc {

class USBMSD_VolumeHandler : public USBMSDHandler, TickerTask {
public:
	USBMSD_VolumeHandler(xpcc::fat::PhysicalVolume *vol) : volume(*vol) {
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

    void handleTick() override;
    void transfer_begins(TransferType type, uint32_t startBlock, int numBlocks) override;
    int disk_read_start(uint8_t * data, uint32_t block, uint32_t blocksLeft) override;
    int disk_write_start(const uint8_t * data, uint32_t block, uint32_t blocksLeft) override;
    int disk_initialize() override;
    uint32_t disk_sectors() override;
    uint16_t disk_sector_size() override;
    int disk_status() override;

protected:
	xpcc::fat::PhysicalVolume& volume;
};

}


#endif /* USBMSD_SDHANDLER_HPP_ */
