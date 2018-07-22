/*
 * DFU.hpp
 *
 *  Created on: Mar 5, 2015
 *      Author: walmis
 */

#ifndef DFU_HPP_
#define DFU_HPP_

#include "USBDevice/USBDevice.h"

#define USB_DFU_DESCRIPTOR_TYPE     0x21
#define USB_DFU_DESCRIPTOR_SIZE     9
#define USB_DFU_SUBCLASS            0x01

/* DFU class-specific requests (Section 3, DFU Rev 1.1) */
#define USB_REQ_DFU_DETACH          0x00
#define USB_REQ_DFU_DNLOAD          0x01
#define USB_REQ_DFU_UPLOAD          0x02
#define USB_REQ_DFU_GETSTATUS       0x03
#define USB_REQ_DFU_CLRSTATUS       0x04
#define USB_REQ_DFU_GETSTATE        0x05
#define USB_REQ_DFU_ABORT           0x06

#define DFU_STATUS_OK               0x00
#define DFU_STATUS_errTARGET        0x01
#define DFU_STATUS_errFILE          0x02
#define DFU_STATUS_errWRITE         0x03
#define DFU_STATUS_errERASE         0x04
#define DFU_STATUS_errCHECK_ERASED  0x05
#define DFU_STATUS_errPROG          0x06
#define DFU_STATUS_errVERIFY        0x07
#define DFU_STATUS_errADDRESS       0x08
#define DFU_STATUS_errNOTDONE       0x09
#define DFU_STATUS_errFIRMWARE      0x0a
#define DFU_STATUS_errVENDOR        0x0b
#define DFU_STATUS_errUSBR          0x0c
#define DFU_STATUS_errPOR           0x0d
#define DFU_STATUS_errUNKNOWN       0x0e
#define DFU_STATUS_errSTALLEDPKT    0x0f

enum dfu_state {
  DFU_STATE_appIDLE             = 0,
  DFU_STATE_appDETACH           = 1,
  DFU_STATE_dfuIDLE             = 2,
  DFU_STATE_dfuDNLOAD_SYNC      = 3,
  DFU_STATE_dfuDNBUSY           = 4,
  DFU_STATE_dfuDNLOAD_IDLE      = 5,
  DFU_STATE_dfuMANIFEST_SYNC    = 6,
  DFU_STATE_dfuMANIFEST         = 7,
  DFU_STATE_dfuMANIFEST_WAIT_RST= 8,
  DFU_STATE_dfuUPLOAD_IDLE      = 9,
  DFU_STATE_dfuERROR            = 10
};

#define DFU_EP0_NONE            0
#define DFU_EP0_UNHANDLED       1
#define DFU_EP0_STALL           2
#define DFU_EP0_ZLP             3
#define DFU_EP0_DATA            4

#define USB_DFU_CAN_DOWNLOAD    (1 << 0)
#define USB_DFU_CAN_UPLOAD      (1 << 1)
#define USB_DFU_MANIFEST_TOL    (1 << 2)
#define USB_DFU_WILL_DETACH     (1 << 3)

#define DFU_SUPPORT
#ifdef DFU_SUPPORT
#define DFU_SIZE (9+9)
#define DFU_INTF_DESC(intf_id) \
	0x09,                                  /* bLength */\
	0x04,                                 /* bDescriptorType */\
	intf_id,                              /* bInterfaceNumber */\
	0x00,                              /* bAlternateSetting */\
	0x00,                              /* bNumEndpoints */\
	0xFE,              /* bInterfaceClass */\
	USB_DFU_SUBCLASS,                  /* bInterfaceSubClass */\
	0x01,                               /* bInterfaceProtocol */\
	0x00,                              /* iInterface */\
	/* DFU RunTime/DFU Mode Functional Descriptor */\
	9,                /* bLength */\
	USB_DFU_DESCRIPTOR_TYPE,           /* bDescriptorType */\
	USB_DFU_CAN_DOWNLOAD | USB_DFU_CAN_UPLOAD | USB_DFU_MANIFEST_TOL | USB_DFU_WILL_DETACH, /* bmAttributes */\
	WBVAL(0x00FF),                     /* wDetachTimeout */\
	WBVAL(64),          /* wTransferSize */\
	WBVAL(0x100),                      /* bcdDFUVersion */\

#else
#define DFU_SIZE 0
#define DFU_INTF_DESC(intf_id)
#endif

namespace xpcc {

class DFUHandler : public USBInterfaceHandler {
public:
	DFUHandler() {
		dfu_status.bStatus = 0;
		dfu_status.bwPollTimeout[0] = 100;
		dfu_status.bState = 0;
		dfu_status.iString = 0;
	}

	virtual void do_detach() = 0;

	bool USBCallback_request() override {
		CONTROL_TRANSFER* t = device->getTransferPtr();
		if(t->setup.bmRequestType.Type & 0b00100001) {
			switch(t->setup.bRequest) {
			case USB_REQ_DFU_GETSTATE:
		        t->ptr = (uint8_t *)&dfu_status.bState; /* Assumes little endian */
		        t->remaining = sizeof(dfu_status.bState);
		        t->direction = DEVICE_TO_HOST;
		        return true;

			case USB_REQ_DFU_GETSTATUS:
		        t->ptr = (uint8_t *)&dfu_status; /* Assumes little endian */
		        t->remaining = sizeof(dfu_status);
		        t->direction = DEVICE_TO_HOST;
			return true;

			case USB_REQ_DFU_DETACH:
				do_detach();
				return true;
			}


		}
		return false;
	}
protected:
	struct {
		uint8_t bStatus;
		uint8_t bwPollTimeout[3];
		uint8_t bState;
		uint8_t iString;
	} __attribute((packed)) dfu_status;
};

}

#endif /* DFU_HPP_ */
