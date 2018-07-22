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
#define CDC2MSD_SER1_BULK_IN  EP2IN
#define CDC2MSD_SER1_BULK_OUT EP2OUT
#define CDC2MSD_SER1_INT_IN   EP1IN

#define CDC2MSD_SER2_BULK_IN  EP8IN
#define CDC2MSD_SER2_BULK_OUT EP8OUT
#define CDC2MSD_SER2_INT_IN   EP10IN

#define CDC2MSD_MSD_BULK_IN  EP5IN
#define CDC2MSD_MSD_BULK_OUT EP5OUT
#endif

#ifdef __ARM_LPC11UXX__
#define CDC2MSD_SER1_BULK_IN  EP1IN
#define CDC2MSD_SER1_BULK_OUT EP1OUT
#define CDC2MSD_SER1_INT_IN   EP4IN

#define CDC2MSD_SER2_BULK_IN  EP2IN
#define CDC2MSD_SER2_BULK_OUT EP2OUT
#define CDC2MSD_SER2_INT_IN   EP4IN

#define CDC2MSD_MSD_EPBULK_IN  EP3IN
#define CDC2MSD_MSD_EPBULK_OUT EP3OUT
#endif

class USB2xCDCMSD: public USBDevice {
public:
	USB2xCDCMSD(USBMSDHandler* msd_handler);

	USBMSDHandler& msd;
	USBSerialHandler serial1;
	USBSerialHandler serial2;

};

}

#endif /* CDCMSD_HPP_ */
