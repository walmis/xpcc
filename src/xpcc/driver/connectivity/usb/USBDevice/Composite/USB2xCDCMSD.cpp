/*
 * USB2xCDCMSD.cpp
 *
 *  Created on: Sep 1, 2017
 *      Author: walmis
 */
#include "USB2xCDCMSD.hpp"
#ifdef CDC2MSD_MSD_EPBULK_OUT
namespace xpcc {

USB_INTERFACE_STRING("Dual CDC + MSD");
USB_PRODUCT_STRING("Dual CDC + MSD Device");

USB2xCDCMSD::USB2xCDCMSD(USBMSDHandler* msd_handler) :
		msd(*msd_handler),
		serial1(CDC2MSD_SER1_BULK_IN, CDC2MSD_SER1_BULK_OUT, CDC2MSD_SER1_INT_IN),
		serial2(CDC2MSD_SER2_BULK_IN, CDC2MSD_SER2_BULK_OUT, CDC2MSD_SER2_INT_IN) {

	this->addInterfaceHandler(serial1);
	this->addInterfaceHandler(serial2);
	msd.setEndpoints(CDC2MSD_MSD_EPBULK_IN, CDC2MSD_MSD_EPBULK_OUT);
	this->addInterfaceHandler(msd);
}

__attribute__((weak))
uint8_t * USBDevice::deviceDesc() {
	static const uint8_t deviceDescriptor[] = {
	DEVICE_DESCRIPTOR_LENGTH, /* bLength */
	DEVICE_DESCRIPTOR, /* bDescriptorType */
	LSB(USB_VERSION_2_0), /* bcdUSB (LSB) */
	MSB(USB_VERSION_2_0), /* bcdUSB (MSB) */
	0xEF, /* bDeviceClass */
	0x02, /* bDeviceSubClass */
	0x01, /* bDeviceprotocol */
	MAX_PACKET_SIZE_EP0, /* bMaxPacketSize0 */
	LSB(USB_VENDOR_ID), /* idVendor (LSB) */
	MSB(USB_VENDOR_ID), /* idVendor (MSB) */
	LSB(USB_PRODUCT_ID), /* idProduct (LSB) */
	MSB(USB_PRODUCT_ID), /* idProduct (MSB) */
	LSB(0), /* bcdDevice (LSB) */
	MSB(0), /* bcdDevice (MSB) */
	STRING_OFFSET_IMANUFACTURER, /* iManufacturer */
	STRING_OFFSET_IPRODUCT, /* iProduct */
	STRING_OFFSET_ISERIAL, /* iSerialNumber */
	0x01 /* bNumConfigurations */
	};
	return (uint8_t*) deviceDescriptor;
}

__attribute__((weak))
uint8_t * USBDevice::configurationDesc() {
	static const uint8_t configDescriptor[] = { 9,                   // bLength;
			2,                      // bDescriptorType;
			LSB(164 + DFU_SIZE),              // wTotalLength
			MSB(164 + DFU_SIZE),
#ifdef DFU_SUPPORT
			6,                      // bNumInterfaces
#else
			5,
#endif
			1,                      // bConfigurationValue
			0,                      // iConfiguration
			0xc0,                   // bmAttributes
			50,                     // bMaxPower

			// IAD
			0x08,//bLength
			0x0B, //bDescriptorType
			0x00, //bFirstInterface
			2, //bInterfaceCount
			0x00, //bFunctionClass
			0x00, //bFunctionSubClass
			0x00, //bFunctionProtocol
			0x00, //iFunction

			/// --------------------------------/////

			// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
			9,// bLength
			4,                      // bDescriptorType
			0,                      // bInterfaceNumber
			0,                      // bAlternateSetting
			1,                      // bNumEndpoints
			0x02,                   // bInterfaceClass
			0x02,                   // bInterfaceSubClass
			0x01,                   // bInterfaceProtocol
			0,                      // iInterface

			// CDC Header Functional Descriptor, CDC Spec 5.2.3.1, Table 26
			5,// bFunctionLength
			0x24,                   // bDescriptorType
			0x00,                   // bDescriptorSubtype
			0x10, 0x01,             // bcdCDC

			// Call Management Functional Descriptor, CDC Spec 5.2.3.2, Table 27
			5,// bFunctionLength
			0x24,                   // bDescriptorType
			0x01,                   // bDescriptorSubtype
			0x03,                   // bmCapabilities
			1,                      // bDataInterface

			// Abstract Control Management Functional Descriptor, CDC Spec 5.2.3.3, Table 28
			4,// bFunctionLength
			0x24,                   // bDescriptorType
			0x02,                   // bDescriptorSubtype
			0x06,                   // bmCapabilities

			// Union Functional Descriptor, CDC Spec 5.2.3.8, Table 33
			5,// bFunctionLength
			0x24,                   // bDescriptorType
			0x06,                   // bDescriptorSubtype
			0,                      // bMasterInterface
			1,                      // bSlaveInterface0

			// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
			ENDPOINT_DESCRIPTOR_LENGTH,// bLength
			ENDPOINT_DESCRIPTOR,            // bDescriptorType
			PHY_TO_DESC(CDC2MSD_SER1_INT_IN),          // bEndpointAddress
			E_INTERRUPT,                    // bmAttributes (0x03=intr)
			LSB(MAX_PACKET_SIZE_EPINT),     // wMaxPacketSize (LSB)
			MSB(MAX_PACKET_SIZE_EPINT),     // wMaxPacketSize (MSB)
			16,                             // bInterval

			// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
			9,// bLength
			4,          // bDescriptorType
			1,          // bInterfaceNumber
			0,          // bAlternateSetting
			2,          // bNumEndpoints
			0x0A,       // bInterfaceClass
			0x00,       // bInterfaceSubClass
			0x00,       // bInterfaceProtocol
			0,          // iInterface

			// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
			7,// bLength
			5,                      // bDescriptorType
			PHY_TO_DESC(CDC2MSD_SER1_BULK_IN), // bEndpointAddress
			0x02,                   // bmAttributes (0x02=bulk)
			LSB(MAX_PACKET_SIZE_EPBULK),    // wMaxPacketSize (LSB)
			MSB(MAX_PACKET_SIZE_EPBULK),    // wMaxPacketSize (MSB)
			0,                      // bInterval

			// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
			7,// bLength
			5,                      // bDescriptorType
			PHY_TO_DESC(CDC2MSD_SER1_BULK_OUT),              // bEndpointAddress
			0x02,                   // bmAttributes (0x02=bulk)
			LSB(MAX_PACKET_SIZE_EPBULK),    // wMaxPacketSize (LSB)
			MSB(MAX_PACKET_SIZE_EPBULK),     // wMaxPacketSize (MSB)
			0,                       // bInterval

			// IAD
			0x08,//bLength
			0x0B, //bDescriptorType
			0x02, //bFirstInterface
			2, //bInterfaceCount
			0x00, //bFunctionClass
			0x00, //bFunctionSubClass
			0x00, //bFunctionProtocol
			0x00, //iFunction

			/// --------------------------------/////
			// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
			9,// bLength
			4,                      // bDescriptorType
			2,                      // bInterfaceNumber
			0,                      // bAlternateSetting
			1,                      // bNumEndpoints
			0x02,                   // bInterfaceClass
			0x02,                   // bInterfaceSubClass
			0x01,                   // bInterfaceProtocol
			0,                      // iInterface

			// CDC Header Functional Descriptor, CDC Spec 5.2.3.1, Table 26
			5,// bFunctionLength
			0x24,                   // bDescriptorType
			0x00,                   // bDescriptorSubtype
			0x10, 0x01,             // bcdCDC

			// Call Management Functional Descriptor, CDC Spec 5.2.3.2, Table 27
			5,// bFunctionLength
			0x24,                   // bDescriptorType
			0x01,                   // bDescriptorSubtype
			0x03,                   // bmCapabilities
			1,                      // bDataInterface

			// Abstract Control Management Functional Descriptor, CDC Spec 5.2.3.3, Table 28
			4,// bFunctionLength
			0x24,                   // bDescriptorType
			0x02,                   // bDescriptorSubtype
			0x06,                   // bmCapabilities

			// Union Functional Descriptor, CDC Spec 5.2.3.8, Table 33
			5,// bFunctionLength
			0x24,                   // bDescriptorType
			0x06,                   // bDescriptorSubtype
			2,                      // bMasterInterface
			3,                      // bSlaveInterface0

			// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
			ENDPOINT_DESCRIPTOR_LENGTH,// bLength
			ENDPOINT_DESCRIPTOR,            // bDescriptorType
			PHY_TO_DESC(CDC2MSD_SER2_INT_IN),          // bEndpointAddress
			E_INTERRUPT,                    // bmAttributes (0x03=intr)
			LSB(MAX_PACKET_SIZE_EPINT),     // wMaxPacketSize (LSB)
			MSB(MAX_PACKET_SIZE_EPINT),     // wMaxPacketSize (MSB)
			16,                             // bInterval

			// interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
			9,// bLength
			4,          // bDescriptorType
			3,          // bInterfaceNumber
			0,          // bAlternateSetting
			2,          // bNumEndpoints
			0x0A,       // bInterfaceClass
			0x00,       // bInterfaceSubClass
			0x00,       // bInterfaceProtocol
			0,          // iInterface

			// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
			7,// bLength
			5,                      // bDescriptorType
			PHY_TO_DESC(CDC2MSD_SER2_BULK_IN), // bEndpointAddress
			0x02,                   // bmAttributes (0x02=bulk)
			LSB(MAX_PACKET_SIZE_EPBULK),    // wMaxPacketSize (LSB)
			MSB(MAX_PACKET_SIZE_EPBULK),    // wMaxPacketSize (MSB)
			0,                      // bInterval

			// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
			7,// bLength
			5,                      // bDescriptorType
			PHY_TO_DESC(CDC2MSD_SER2_BULK_OUT),              // bEndpointAddress
			0x02,                   // bmAttributes (0x02=bulk)
			LSB(MAX_PACKET_SIZE_EPBULK),    // wMaxPacketSize (LSB)
			MSB(MAX_PACKET_SIZE_EPBULK),     // wMaxPacketSize (MSB)
			0,                       // bInterval

			/// --------------------------------/////

			// Interface 2, Alternate Setting 0, MSC Class
			9,// bLength
			4,      // bDescriptorType
			0x04,   // bInterfaceNumber
			0x00,   // bAlternateSetting
			0x02,   // bNumEndpoints
			0x08,   // bInterfaceClass
			0x06,   // bInterfaceSubClass
			0x50,   // bInterfaceProtocol
			0x04,   // iInterface

			// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
			7,// bLength
			5,                          // bDescriptorType
			PHY_TO_DESC(CDC2MSD_MSD_EPBULK_IN),     // bEndpointAddress
			0x02,                       // bmAttributes (0x02=bulk)
			LSB(MAX_PACKET_SIZE_EPBULK),                 // wMaxPacketSize (LSB)
			MSB(MAX_PACKET_SIZE_EPBULK),                 // wMaxPacketSize (MSB)
			0,                          // bInterval

			// endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
			7,// bLength
			5,                          // bDescriptorType
			PHY_TO_DESC(CDC2MSD_MSD_EPBULK_OUT),    // bEndpointAddress
			0x02,                       // bmAttributes (0x02=bulk)
			LSB(MAX_PACKET_SIZE_EPBULK),                 // wMaxPacketSize (LSB)
			MSB(MAX_PACKET_SIZE_EPBULK),                 // wMaxPacketSize (MSB)
			0,                           // bInterval

			DFU_INTF_DESC(0x05) };
	return (uint8_t*) configDescriptor;
}

}
#endif
