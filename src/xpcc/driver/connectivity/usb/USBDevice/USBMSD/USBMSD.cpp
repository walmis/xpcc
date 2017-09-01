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

#include "stdint.h"
#include "USBMSD.h"
#include "../DFU.hpp"
namespace xpcc {

USB_PRODUCT_STRING("XPCC Mass Storage");
USB_INTERFACE_STRING("MSD");

USBMSD::USBMSD(USBMSDHandler* msd) {
		this->addInterfaceHandler(*msd);
		msd->setEndpoints(MSD_EPBULK_IN, MSD_EPBULK_OUT);
};

uint8_t * USBDevice::configurationDesc() {
    static const uint8_t configDescriptor[] = {

        // Configuration 1
        9,      // bLength
        2,      // bDescriptorType
        LSB(9 + 9 + 7 + 7 + DFU_SIZE), // wTotalLength
        MSB(9 + 9 + 7 + 7 + DFU_SIZE),
        0x02,   // bNumInterfaces
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
        0,                           // bInterval,

		DFU_INTF_DESC(1)
    };
    return (uint8_t*)configDescriptor;
}
}
