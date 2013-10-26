/* Copyright (c) 2010-2011 mbed.org, MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#ifndef USBMSD_H
#define USBMSD_H

/* These headers are included for child class. */

#include "../USBDevice/USBDevice.h"
#include "USBMSDHandler.h"


/**
 * USBMSD class: generic class in order to use all kinds of blocks storage chip
 *
 * Introduction
 *
 * The USBMSD implements the MSD protocol. It permits to access a memory chip (flash, sdcard,...)
 * from a computer over USB. But this class doesn't work standalone, you need to subclass this class
 * and define virtual functions which are called in USBMSD.
 *
 * How to use this class with your chip ?
 *
 * You have to inherit and define some pure virtual functions (mandatory step):
 *   - virtual int disk_read(char * data, int block): function to read a block
 *   - virtual int disk_write(const char * data, int block): function to write a block
 *   - virtual int disk_initialize(): function to initialize the memory
 *   - virtual int disk_sectors(): return the number of blocks
 *   - virtual int disk_size(): return the memory size
 *   - virtual int disk_status(): return the status of the storage chip (0: OK, 1: not initialized, 2: no medium in the drive, 4: write protection)
 *
 * All functions names are compatible with the fat filesystem library. So you can imagine using your own class with
 * USBMSD and the fat filesystem library in the same program. Just be careful because there are two different parts which
 * will access the sd card. You can do a master/slave system using the disk_status method.
 *
 * Once these functions defined, you can call connect() (at the end of the constructor of your class for instance)
 * of USBMSD to connect your mass storage device. connect() will first call disk_status() to test the status of the disk.
 * If disk_status() returns 1 (disk not initialized), then disk_initialize() is called. After this step, connect() will collect information
 * such as the number of blocks and the memory size.
 */

namespace xpcc {

class USBMSD: public USBDevice {
public:

    /**
    * Constructor
    *
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your preoduct_release
    */
	USBMSD(uint16_t vendor_id = 0x0703, uint16_t product_id = 0x0104,
			uint16_t product_release = 0x0001);


	void assignHandler(USBMSDHandler &handler) {
		this->addInterfaceHandler(handler);
	}


protected:

    /*
    * Get string product descriptor
    *
    * @returns pointer to the string product descriptor
    */
    virtual uint8_t * stringIproductDesc();

    /*
    * Get string interface descriptor
    *
    * @returns pointer to the string interface descriptor
    */
    virtual uint8_t * stringIinterfaceDesc();

    /*
    * Get configuration descriptor
    *
    * @returns pointer to the configuration descriptor
    */
    virtual uint8_t * configurationDesc();


};

}
#endif
