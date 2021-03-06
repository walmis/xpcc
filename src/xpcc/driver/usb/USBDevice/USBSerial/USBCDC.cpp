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
#include "USBCDC.h"

using namespace xpcc;

#define DEFAULT_CONFIGURATION (1)

#define CDC_SET_LINE_CODING        0x20
#define CDC_GET_LINE_CODING        0x21
#define CDC_SET_CONTROL_LINE_STATE 0x22

#define MAX_CDC_REPORT_SIZE MAX_PACKET_SIZE_EPBULK

__attribute__((weak))
USB_INTERFACE_STRING("CDC");

__attribute__((weak))
USB_PRODUCT_STRING("CDC Device");

USBCDC::USBCDC() {}

__attribute__((weak))
uint8_t * USBDevice::deviceDesc() {
	static const uint8_t deviceDescriptor[] = {
	    18,                   // bLength
	    1,                    // bDescriptorType
	    0x10, 0x01,           // bcdUSB
	    2,                    // bDeviceClass
	    0,                    // bDeviceSubClass
	    0,                    // bDeviceProtocol
	    MAX_PACKET_SIZE_EP0,  // bMaxPacketSize0
	    LSB(USB_VENDOR_ID), MSB(USB_VENDOR_ID),  // idVendor
	    LSB(USB_PRODUCT_ID), MSB(USB_PRODUCT_ID),// idProduct
	    0x00, 0x01,           // bcdDevice
	    1,                    // iManufacturer
	    2,                    // iProduct
	    3,                    // iSerialNumber
	    1                     // bNumConfigurations
	};
    return (uint8_t*)deviceDescriptor;
}

#define CONFIG1_DESC_SIZE (9+8+9+5+5+4+5+7+9+7+7+DFU_SIZE)
__attribute__((weak))
uint8_t * USBDevice::configurationDesc() {
	 static const uint8_t configDescriptor[] = {
	        // configuration descriptor
	        9,                      // bLength
	        2,                      // bDescriptorType
	        LSB(CONFIG1_DESC_SIZE), // wTotalLength
	        MSB(CONFIG1_DESC_SIZE),
	        3,                      // bNumInterfaces
	        1,                      // bConfigurationValue
	        0,                      // iConfiguration
	        0x80,                   // bmAttributes
	        50,                     // bMaxPower

	        // IAD to associate the two CDC interfaces
	        0x08,                   // bLength
	        0x0b,                   // bDescriptorType
	        0x00,                   // bFirstInterface
	        0x02,                   // bInterfaceCount
	        0x02,                   // bFunctionClass
	        0x02,                   // bFunctionSubClass
	        0,                      // bFunctionProtocol
	        0,                      // iFunction

	        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
	        9,                      // bLength
	        4,                      // bDescriptorType
	        0,                      // bInterfaceNumber
	        0,                      // bAlternateSetting
	        1,                      // bNumEndpoints
	        0x02,                   // bInterfaceClass
	        0x02,                   // bInterfaceSubClass
	        0x01,                   // bInterfaceProtocol
	        0,                      // iInterface

	        // CDC Header Functional Descriptor, CDC Spec 5.2.3.1, Table 26
	        5,                      // bFunctionLength
	        0x24,                   // bDescriptorType
	        0x00,                   // bDescriptorSubtype
	        0x10, 0x01,             // bcdCDC

	        // Call Management Functional Descriptor, CDC Spec 5.2.3.2, Table 27
	        5,                      // bFunctionLength
	        0x24,                   // bDescriptorType
	        0x01,                   // bDescriptorSubtype
	        0x03,                   // bmCapabilities
	        1,                      // bDataInterface

	        // Abstract Control Management Functional Descriptor, CDC Spec 5.2.3.3, Table 28
	        4,                      // bFunctionLength
	        0x24,                   // bDescriptorType
	        0x02,                   // bDescriptorSubtype
	        0x06,                   // bmCapabilities

	        // Union Functional Descriptor, CDC Spec 5.2.3.8, Table 33
	        5,                      // bFunctionLength
	        0x24,                   // bDescriptorType
	        0x06,                   // bDescriptorSubtype
	        0,                      // bMasterInterface
	        1,                      // bSlaveInterface0

	        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	        ENDPOINT_DESCRIPTOR_LENGTH,     // bLength
	        ENDPOINT_DESCRIPTOR,            // bDescriptorType
	        PHY_TO_DESC(CDC_EPINT_IN),          // bEndpointAddress
	        E_INTERRUPT,                    // bmAttributes (0x03=intr)
	        LSB(MAX_PACKET_SIZE_EPINT),     // wMaxPacketSize (LSB)
	        MSB(MAX_PACKET_SIZE_EPINT),     // wMaxPacketSize (MSB)
	        16,                             // bInterval

	        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
	        9,                          // bLength
	        4,                          // bDescriptorType
	        1,                          // bInterfaceNumber
	        0,                          // bAlternateSetting
	        2,                          // bNumEndpoints
	        0x0A,                       // bInterfaceClass
	        0x00,                       // bInterfaceSubClass
	        0x00,                       // bInterfaceProtocol
	        0,                          // iInterface

	        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
	        ENDPOINT_DESCRIPTOR,        // bDescriptorType
	        PHY_TO_DESC(CDC_EPBULK_IN),     // bEndpointAddress
	        E_BULK,                     // bmAttributes (0x02=bulk)
	        LSB(MAX_PACKET_SIZE_EPBULK),// wMaxPacketSize (LSB)
	        MSB(MAX_PACKET_SIZE_EPBULK),// wMaxPacketSize (MSB)
	        0,                          // bInterval

	        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
	        ENDPOINT_DESCRIPTOR,        // bDescriptorType
	        PHY_TO_DESC(CDC_EPBULK_OUT),    // bEndpointAddress
	        E_BULK,                     // bmAttributes (0x02=bulk)
	        LSB(MAX_PACKET_SIZE_EPBULK),// wMaxPacketSize (LSB)
	        MSB(MAX_PACKET_SIZE_EPBULK),// wMaxPacketSize (MSB)
	        0,                           // bInterval

	        DFU_INTF_DESC(2)

	    };
	    return (uint8_t*)configDescriptor;
}
#undef CONFIG1_DESC_SIZE
