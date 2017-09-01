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

#ifdef __ARM_LPC11UXX__
#define EP5IN EP4IN
#define EP5OUT EP4OUT
#endif

#ifndef CDCMSD_CDC_EPINT_IN
#define CDCMSD_CDC_EPINT_IN EP1IN
#endif

#ifndef CDCMSD_CDC_EPBULK_IN
#define CDCMSD_CDC_EPBULK_IN EP2IN
#endif

#ifndef CDCMSD_CDC_EPBULK_OUT
#define CDCMSD_CDC_EPBULK_OUT EP2OUT
#endif

#ifndef CDCMSD_MSD_EPBULK_IN
#define CDCMSD_MSD_EPBULK_IN EP3IN
#endif

#ifndef CDCMSD_MSD_EPBULK_OUT
#define CDCMSD_MSD_EPBULK_OUT EP3OUT
#endif

namespace xpcc {


class USBCDCMSD: public USBDevice, public IODevice {
public:
	//template <typename ... Args>
	USBCDCMSD(USBMSDHandler* msd);

	//override IODevice methods
	size_t
	write(char c) override {
		serial.putch(c);
		return 1;
	}

	void
	flush() override {}

	/// Read a single character
	int16_t
	read() override {
		return serial.getch();
	}

	USBSerialHandler serial;
	USBMSDHandler& msd;
};

}

#endif /* CDCMSD_HPP_ */
