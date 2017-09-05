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

#ifndef USBDESC
#define USBDESC

/* Standard descriptor types */
#define DEVICE_DESCRIPTOR        (1)
#define CONFIGURATION_DESCRIPTOR (2)
#define STRING_DESCRIPTOR        (3)
#define INTERFACE_DESCRIPTOR     (4)
#define ENDPOINT_DESCRIPTOR      (5)
#define QUALIFIER_DESCRIPTOR     (6)

/* Standard descriptor lengths */
#define DEVICE_DESCRIPTOR_LENGTH        (0x12)
#define CONFIGURATION_DESCRIPTOR_LENGTH (0x09)
#define INTERFACE_DESCRIPTOR_LENGTH     (0x09)
#define ENDPOINT_DESCRIPTOR_LENGTH      (0x07)


/*string offset*/
#define STRING_OFFSET_LANGID            (0) 
#define STRING_OFFSET_IMANUFACTURER     (1)
#define STRING_OFFSET_IPRODUCT          (2)
#define STRING_OFFSET_ISERIAL           (3)
#define STRING_OFFSET_ICONFIGURATION    (4)
#define STRING_OFFSET_IINTERFACE        (5)

/* USB Specification Release Number */
#define USB_VERSION_2_0 (0x0200)

/* Least/Most significant byte of short integer */
constexpr uint8_t LSB(uint32_t n) { return n&0xff; }
constexpr uint8_t MSB(uint32_t n) { return (((n)&0xff00)>>8); }
//#define LSB(n)  ((static_cast<uint8_t>(n))&0xff)
//#define MSB(n)  (((n)&0xff00)>>8)

#define WBVAL(x) ((x) & 0xFF),(((x) >> 8) & 0xFF)

/* Convert physical endpoint number to descriptor endpoint number */
#define PHY_TO_DESC(endpoint) (((endpoint)>>1) | (((endpoint) & 1) ? 0x80:0))

/* bmAttributes in configuration descriptor */
/* C_RESERVED must always be set */
#define C_RESERVED      (1U<<7)
#define C_SELF_POWERED  (1U<<6)
#define C_REMOTE_WAKEUP (1U<<5)

/* bMaxPower in configuration descriptor */
#define C_POWER(mA)     ((mA)/2)

/* bmAttributes in endpoint descriptor */
#define E_CONTROL       (0x00)
#define E_ISOCHRONOUS   (0x01)
#define E_BULK          (0x02)
#define E_INTERRUPT     (0x03)

/* For isochronous endpoints only: */
#define E_NO_SYNCHRONIZATION    (0x00)
#define E_ASYNCHRONOUS          (0x04)
#define E_ADAPTIVE              (0x08)
#define E_SYNCHRONOUS           (0x0C)
#define E_DATA                  (0x00)
#define E_FEEDBACK              (0x10)
#define E_IMPLICIT_FEEDBACK     (0x20)

#ifndef USB_VENDOR_ID
#define USB_VENDOR_ID 0xffff
#endif

#ifndef USB_PRODUCT_ID
#define USB_PRODUCT_ID 0xaaaa
#endif

#define USB_STRING(name, str) static const struct { \
				uint8_t size = sizeof(CONCAT(u, str)) + 2; \
				uint8_t id = 3; \
			    char16_t s[sizeof(CONCAT(u, str))] = CONCAT(u, str);\
			  } name


#define USB_INTERFACE_STRING(text) \
		uint8_t * xpcc::USBDevice::stringIinterfaceDesc() { \
		    USB_STRING(stringIinterfaceDescriptor, text);\
		    return (uint8_t*)&stringIinterfaceDescriptor; \
		} \

#define USB_PRODUCT_STRING(text) \
		uint8_t * xpcc::USBDevice::stringIproductDesc() { \
		    USB_STRING(stringIproductDesc, text);\
		    return (uint8_t*)&stringIproductDesc; \
		} \

#define USB_MANUFACTURER_STRING(text) \
		uint8_t * xpcc::USBDevice::stringImanufacturerDesc() { \
			USB_STRING(stringImanufacturerDesc, text);\
			return (uint8_t*)&stringImanufacturerDesc; \
		} \

#define USB_SERIAL_STRING(text) \
		uint8_t * xpcc::USBDevice::stringIserialDesc() { \
			USB_STRING(stringIserialDesc, text);\
			return (uint8_t*)&stringIserialDesc; \
		} \

#define USB_CONFIGURATION_STRING(text) \
		uint8_t * xpcc::USBDevice::stringIConfigurationDesc() { \
			USB_STRING(stringIConfigurationDesc, text);\
			return (uint8_t*)&stringIConfigurationDesc; \
		} \

#endif
