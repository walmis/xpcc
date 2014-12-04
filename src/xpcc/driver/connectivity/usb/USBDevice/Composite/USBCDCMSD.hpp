/*
 * CDCMSD.hpp
 *
 *  Created on: Nov 2, 2013
 *      Author: walmis
 */

#ifndef CDCMSD_HPP_
#define CDCMSD_HPP_

#include "../USBDevice/USBDevice.h"
#include "../USBSerial/USBSerialHandler.hpp"
#include "../USBMSD/USBMSDHandler.h"

namespace xpcc {

template<class MSDHandlerClass = USBMSDHandler, class SerialHandlerClass = USBSerialHandler>

class USBCDCMSD: public USBDevice, public IODevice {
public:
	template <typename ... Args>
	USBCDCMSD(uint16_t vendor_id,
			uint16_t product_id, uint16_t product_release, Args... args) :
			USBDevice(vendor_id, product_id, product_release),
			msd(args..., EP5IN, EP5OUT),
			serial(EPBULK_IN, EPBULK_OUT, EPINT_IN) {

		this->addInterfaceHandler(serial);
		this->addInterfaceHandler(msd);
	}

	//override IODevice methods
	size_t
	write(char c) override {
		serial.putc(c);
		return 1;
	}

	void
	flush() override {}

	/// Read a single character
	int16_t
	read() override {
		return serial.getc();
	}

	uint8_t * stringIinterfaceDesc() override {
	    static const uint8_t stringIinterfaceDescriptor[] = {
	        0x0e,
	        STRING_DESCRIPTOR,
	        'C',0,'D',0,'C',0,'M',0,'S',0,'C',0,
	    };
	    return (uint8_t*)stringIinterfaceDescriptor;
	}

	uint8_t * stringIproductDesc() override {
	    static const uint8_t stringIproductDescriptor[] = {
	        0x1c,
	        STRING_DESCRIPTOR,
	        'C',0,'D',0,'C',0,'M',0,'S',0,'D',0,' ',0,'D',0,'E',0,'V',0,'I',0,'C',0,'E',0
	    };
	    return (uint8_t*)stringIproductDescriptor;
	}


	uint8_t * configurationDesc() override {
	    static const uint8_t configDescriptor[] = {
	        9,                      // bLength;
	        2,                      // bDescriptorType;
	        LSB(0x62),              // wTotalLength
	        MSB(0x62),
	        3,                      // bNumInterfaces
	        1,                      // bConfigurationValue
	        0,                      // iConfiguration
	        0xc0,                   // bmAttributes
	        50,                     // bMaxPower

	        // IAD
	//        0x08, 0x0B, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00,
	        0x08, 0x0B, 0x00, 0x02, 0x02, 0x02, 0x01, 0x00,

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
	        PHY_TO_DESC(EPINT_IN),          // bEndpointAddress
	        E_INTERRUPT,                    // bmAttributes (0x03=intr)
	        LSB(MAX_PACKET_SIZE_EPINT),     // wMaxPacketSize (LSB)
	        MSB(MAX_PACKET_SIZE_EPINT),     // wMaxPacketSize (MSB)
	        16,                             // bInterval


	        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
	        9,          // bLength
	        4,          // bDescriptorType
	        1,          // bInterfaceNumber
	        0,          // bAlternateSetting
	        2,          // bNumEndpoints
	        0x0A,       // bInterfaceClass
	        0x00,       // bInterfaceSubClass
	        0x00,       // bInterfaceProtocol
	        0,          // iInterface

	        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	        7,                      // bLength
	        5,                      // bDescriptorType
	        PHY_TO_DESC(EPBULK_IN), // bEndpointAddress
	        0x02,                   // bmAttributes (0x02=bulk)
	        LSB(MAX_PACKET_SIZE_EPBULK),    // wMaxPacketSize (LSB)
	        MSB(MAX_PACKET_SIZE_EPBULK),    // wMaxPacketSize (MSB)
	        0,                      // bInterval

	        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	        7,                      // bLength
	        5,                      // bDescriptorType
	        PHY_TO_DESC(EPBULK_OUT),// bEndpointAddress
	        0x02,                   // bmAttributes (0x02=bulk)
	        LSB(MAX_PACKET_SIZE_EPBULK),    // wMaxPacketSize (LSB)
	        MSB(MAX_PACKET_SIZE_EPBULK),     // wMaxPacketSize (MSB)
	        0,                       // bInterval

	        // Interface 2, Alternate Setting 0, MSC Class
	        9,      // bLength
	        4,      // bDescriptorType
	        0x02,   // bInterfaceNumber
	        0x00,   // bAlternateSetting
	        0x02,   // bNumEndpoints
	        0x08,   // bInterfaceClass
	        0x06,   // bInterfaceSubClass
	        0x50,   // bInterfaceProtocol
	        0x04,   // iInterface

	        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	        7,                          // bLength
	        5,                          // bDescriptorType
	        PHY_TO_DESC(EP5IN),     // bEndpointAddress
	        0x02,                       // bmAttributes (0x02=bulk)
	        LSB(MAX_PACKET_SIZE_EPBULK),// wMaxPacketSize (LSB)
	        MSB(MAX_PACKET_SIZE_EPBULK),// wMaxPacketSize (MSB)
	        0,                          // bInterval

	        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	        7,                          // bLength
	        5,                          // bDescriptorType
	        PHY_TO_DESC(EP5OUT),    // bEndpointAddress
	        0x02,                       // bmAttributes (0x02=bulk)
	        LSB(MAX_PACKET_SIZE_EPBULK),// wMaxPacketSize (LSB)
	        MSB(MAX_PACKET_SIZE_EPBULK),// wMaxPacketSize (MSB)
	        0                           // bInterval
	    };
	    return (uint8_t*)configDescriptor;
	}

	MSDHandlerClass msd;
	SerialHandlerClass serial;

};

}

#endif /* CDCMSD_HPP_ */
