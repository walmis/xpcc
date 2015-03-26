/*
 * CDCMSD.hpp
 *
 *  Created on: Nov 2, 2013
 *      Author: walmis
 */

#ifndef x2CDCMSD_HPP_
#define x2CDCMSD_HPP_

#include "../USBDevice/USBDevice.h"
#include "../USBSerial/USBSerialHandler.hpp"
#include "../USBMSD/USBMSDHandler.h"
#include "../DFU.hpp"

namespace xpcc {

#ifdef __ARM_LPC17XX__
#define S1_BULK_IN  EP2IN
#define S1_BULK_OUT EP2OUT
#define S1_INT_IN   EP1IN

#define S2_BULK_IN  EP8IN
#define S2_BULK_OUT EP8OUT
#define S2_INT_IN   EP10IN

#define MSD_BULK_IN  EP5IN
#define MSD_BULK_OUT EP5OUT
#endif
#ifdef __ARM_LPC11UXX__
#define S1_BULK_IN  EP1IN
#define S1_BULK_OUT EP1OUT
#define S1_INT_IN   EP4IN

#define S2_BULK_IN  EP2IN
#define S2_BULK_OUT EP2OUT
#define S2_INT_IN   EP4IN

#define MSD_BULK_IN  EP3IN
#define MSD_BULK_OUT EP3OUT
#endif

class USB2xCDCMSD: public USBDevice {
public:
	USB2xCDCMSD(USBMSDHandler* msd_handler, uint16_t vendor_id,
			uint16_t product_id, uint16_t product_release) :
			USBDevice(vendor_id, product_id, product_release),
			msd(*msd_handler),
			serial1(S1_BULK_IN, S1_BULK_OUT, S1_INT_IN),
			serial2(S2_BULK_IN, S2_BULK_OUT, S2_INT_IN) {

		this->addInterfaceHandler(serial1);
		this->addInterfaceHandler(serial2);

		msd.setEndpoints(MSD_BULK_IN, MSD_BULK_OUT);
		this->addInterfaceHandler(msd);
	}

	USBMSDHandler& msd;
	USBSerialHandler serial1;
	USBSerialHandler serial2;

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

	uint8_t * deviceDesc() {
	    static const uint8_t deviceDescriptor[] = {
	        DEVICE_DESCRIPTOR_LENGTH,       /* bLength */
	        DEVICE_DESCRIPTOR,              /* bDescriptorType */
	        LSB(USB_VERSION_2_0),           /* bcdUSB (LSB) */
	        MSB(USB_VERSION_2_0),           /* bcdUSB (MSB) */
	        0xEF,                           /* bDeviceClass */
	        0x02,                           /* bDeviceSubClass */
	        0x01,                           /* bDeviceprotocol */
	        MAX_PACKET_SIZE_EP0,            /* bMaxPacketSize0 */
	        LSB(VENDOR_ID),                 /* idVendor (LSB) */
	        MSB(VENDOR_ID),                 /* idVendor (MSB) */
	        LSB(PRODUCT_ID),                /* idProduct (LSB) */
	        MSB(PRODUCT_ID),                /* idProduct (MSB) */
	        LSB(PRODUCT_RELEASE),           /* bcdDevice (LSB) */
	        MSB(PRODUCT_RELEASE),           /* bcdDevice (MSB) */
	        STRING_OFFSET_IMANUFACTURER,    /* iManufacturer */
	        STRING_OFFSET_IPRODUCT,         /* iProduct */
	        STRING_OFFSET_ISERIAL,          /* iSerialNumber */
	        0x01                            /* bNumConfigurations */
	    };
	    return (uint8_t*)deviceDescriptor;
	}

	uint8_t * configurationDesc() override {
	    static const uint8_t configDescriptor[] = {
	        9,                      // bLength;
	        2,                      // bDescriptorType;
	        LSB(164+DFU_SIZE),              // wTotalLength
	        MSB(164+DFU_SIZE),
	        6,                      // bNumInterfaces
	        1,                      // bConfigurationValue
	        0,                      // iConfiguration
	        0xc0,                   // bmAttributes
	        50,                     // bMaxPower

	        // IAD
	        0x08, //bLength
			0x0B, //bDescriptorType
			0x00, //bFirstInterface
			2, //bInterfaceCount
			0x00, //bFunctionClass
			0x00, //bFunctionSubClass
			0x00, //bFunctionProtocol
			0x00, //iFunction

			/// --------------------------------/////

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
	        PHY_TO_DESC(S1_INT_IN),          // bEndpointAddress
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
	        PHY_TO_DESC(S1_BULK_IN), // bEndpointAddress
	        0x02,                   // bmAttributes (0x02=bulk)
	        LSB(MAX_PACKET_SIZE_EPBULK),    // wMaxPacketSize (LSB)
	        MSB(MAX_PACKET_SIZE_EPBULK),    // wMaxPacketSize (MSB)
	        0,                      // bInterval

	        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	        7,                      // bLength
	        5,                      // bDescriptorType
	        PHY_TO_DESC(S1_BULK_OUT),// bEndpointAddress
	        0x02,                   // bmAttributes (0x02=bulk)
	        LSB(MAX_PACKET_SIZE_EPBULK),    // wMaxPacketSize (LSB)
	        MSB(MAX_PACKET_SIZE_EPBULK),     // wMaxPacketSize (MSB)
	        0,                       // bInterval

	        // IAD
	        0x08, //bLength
			0x0B, //bDescriptorType
			0x02, //bFirstInterface
			2, //bInterfaceCount
			0x00, //bFunctionClass
			0x00, //bFunctionSubClass
			0x00, //bFunctionProtocol
			0x00, //iFunction

			/// --------------------------------/////
 // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
			9,                      // bLength
			4,                      // bDescriptorType
			2,                      // bInterfaceNumber
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
			2,                      // bMasterInterface
			3,                      // bSlaveInterface0

			// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
			ENDPOINT_DESCRIPTOR_LENGTH,     // bLength
			ENDPOINT_DESCRIPTOR,            // bDescriptorType
			PHY_TO_DESC(S2_INT_IN),          // bEndpointAddress
			E_INTERRUPT,                    // bmAttributes (0x03=intr)
			LSB(MAX_PACKET_SIZE_EPINT),     // wMaxPacketSize (LSB)
			MSB(MAX_PACKET_SIZE_EPINT),     // wMaxPacketSize (MSB)
			16,                             // bInterval

			// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
			9,          // bLength
			4,          // bDescriptorType
			3,          // bInterfaceNumber
			0,          // bAlternateSetting
			2,          // bNumEndpoints
			0x0A,       // bInterfaceClass
			0x00,       // bInterfaceSubClass
			0x00,       // bInterfaceProtocol
			0,          // iInterface

			// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
			7,                      // bLength
			5,                      // bDescriptorType
			PHY_TO_DESC(S2_BULK_IN), // bEndpointAddress
			0x02,                   // bmAttributes (0x02=bulk)
			LSB(MAX_PACKET_SIZE_EPBULK),    // wMaxPacketSize (LSB)
			MSB(MAX_PACKET_SIZE_EPBULK),    // wMaxPacketSize (MSB)
			0,                      // bInterval

			// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
			7,                      // bLength
			5,                      // bDescriptorType
			PHY_TO_DESC(S2_BULK_OUT),// bEndpointAddress
			0x02,                   // bmAttributes (0x02=bulk)
			LSB(MAX_PACKET_SIZE_EPBULK),    // wMaxPacketSize (LSB)
			MSB(MAX_PACKET_SIZE_EPBULK),     // wMaxPacketSize (MSB)
			0,                       // bInterval

			/// --------------------------------/////

	        // Interface 2, Alternate Setting 0, MSC Class
	        9,      // bLength
	        4,      // bDescriptorType
	        0x04,   // bInterfaceNumber
	        0x00,   // bAlternateSetting
	        0x02,   // bNumEndpoints
	        0x08,   // bInterfaceClass
	        0x06,   // bInterfaceSubClass
	        0x50,   // bInterfaceProtocol
	        0x04,   // iInterface

	        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	        7,                          // bLength
	        5,                          // bDescriptorType
	        PHY_TO_DESC(MSD_BULK_IN),     // bEndpointAddress
	        0x02,                       // bmAttributes (0x02=bulk)
	        LSB(MAX_PACKET_SIZE_EPBULK),// wMaxPacketSize (LSB)
	        MSB(MAX_PACKET_SIZE_EPBULK),// wMaxPacketSize (MSB)
	        0,                          // bInterval

	        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
	        7,                          // bLength
	        5,                          // bDescriptorType
	        PHY_TO_DESC(MSD_BULK_OUT),    // bEndpointAddress
	        0x02,                       // bmAttributes (0x02=bulk)
	        LSB(MAX_PACKET_SIZE_EPBULK),// wMaxPacketSize (LSB)
	        MSB(MAX_PACKET_SIZE_EPBULK),// wMaxPacketSize (MSB)
	        0,                           // bInterval

	        DFU_INTF_DESC(0x05)
	    };
	    return (uint8_t*)configDescriptor;
	}

};

}

#endif /* CDCMSD_HPP_ */
