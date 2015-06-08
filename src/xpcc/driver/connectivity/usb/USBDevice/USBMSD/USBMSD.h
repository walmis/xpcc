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


#ifndef MSD_EPBULK_IN
#define MSD_EPBULK_IN EPBULK_IN
#endif

#ifndef MSD_EPBULK_OUT
#define MSD_EPBULK_OUT EPBULK_OUT
#endif


#ifndef USBMSD_H
#define USBMSD_H

/* These headers are included for child class. */

#include "../USBDevice/USBDevice.h"
#include "USBMSDHandler.h"


namespace xpcc {

  
/**
* \brief		USB MSD Class

* \ingroup		usb
*/  

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


/*! 
 * \tparam USBMSDHandler subclass that implements disk access methods
 */

class USBMSDHandler;

class USBMSD: public USBDevice {
public:

    /**
    * Constructor
    *
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your preoduct_release
    */
	USBMSD(USBMSDHandler* msd, uint16_t vendor_id = 0x0703, uint16_t product_id = 0x0104,
			uint16_t product_release = 0x0001);


protected:
	uint8_t * stringIinterfaceDesc() {
	    static const uint8_t stringIinterfaceDescriptor[] = {
	        0x08,                           //bLength
	        STRING_DESCRIPTOR,              //bDescriptorType 0x03
	        'M',0,'S',0,'D',0               //bString iInterface - MSD
	    };
	    return (uint8_t*)stringIinterfaceDescriptor;
	}

	uint8_t * stringIproductDesc() {
	    static const uint8_t stringIproductDescriptor[] = {
	        0x12,                                           //bLength
	        STRING_DESCRIPTOR,                              //bDescriptorType 0x03
	        'x',0,'p',0,'c',0,'c',0,' ',0,'M',0,'S',0,'D',0 //bString iProduct - Mbed Audio
	    };
	    return (uint8_t*)stringIproductDescriptor;
	}

	uint8_t * configurationDesc() {
	    static const uint8_t configDescriptor[] = {

	        // Configuration 1
	        9,      // bLength
	        2,      // bDescriptorType
	        LSB(9 + 9 + 7 + 7), // wTotalLength
	        MSB(9 + 9 + 7 + 7),
	        0x01,   // bNumInterfaces
	        0x01,   // bConfigurationValue: 0x01 is used to select this configuration
	        0x00,   // iConfiguration: no string to describe this configuration
	        0xC0,   // bmAttributes
	        100,    // bMaxPower, device power consumption is 100 mA

	        // Interface 0, Alternate Setting 0, MSC Class
	        9,      // bLength
	        4,      // bDescriptorType
	        0x00,   // bInterfaceNumber
	        0x00,   // bAlternateSetting
	        0x02,   // bNumEndpoints
	        0x08,   // bInterfaceClass
	        0x06,   // bInterfaceSubClass
	        0x50,   // bInterfaceProtocol
	        0x04,   // iInterface

	        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	        7,                          // bLength
	        5,                          // bDescriptorType
	        PHY_TO_DESC(MSD_EPBULK_IN),     // bEndpointAddress
	        0x02,                       // bmAttributes (0x02=bulk)
	        LSB(MAX_PACKET_SIZE_EPBULK),// wMaxPacketSize (LSB)
	        MSB(MAX_PACKET_SIZE_EPBULK),// wMaxPacketSize (MSB)
	        0,                          // bInterval

	        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	        7,                          // bLength
	        5,                          // bDescriptorType
	        PHY_TO_DESC(MSD_EPBULK_OUT),    // bEndpointAddress
	        0x02,                       // bmAttributes (0x02=bulk)
	        LSB(MAX_PACKET_SIZE_EPBULK),// wMaxPacketSize (LSB)
	        MSB(MAX_PACKET_SIZE_EPBULK),// wMaxPacketSize (MSB)
	        0                           // bInterval
	    };
	    return (uint8_t*)configDescriptor;
	}

};

}
#endif
