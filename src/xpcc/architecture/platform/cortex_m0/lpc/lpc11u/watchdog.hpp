/*
 * watchdog.hpp
 *
 *  Created on: Oct 18, 2013
 *      Author: walmis
 */

#ifndef WATCHDOG_HPP_
#define WATCHDOG_HPP_

#include <xpcc/architecture.hpp>


namespace xpcc {

namespace lpc11u {

#define WDEN              (0x1<<0)
#define WDRESET           (0x1<<1)
#define WDTOF             (0x1<<2)
#define WDINT             (0x1<<3)
#define WDPROTECT         (0x1<<4)

class Watchdog {
public:
	//default 100ms
	static void init(uint32_t timeout_us = 100*1000) {

		  // Settings for LPC1114
		  /* Enable clock to WDT */
		  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<15);

		  LPC_SYSCON->WDTOSCCTRL = 0x03F; /* ~8kHz */
		  LPC_SYSCON->PDRUNCFG &= ~(0x1<<6);

		  LPC_WWDT->CLKSEL = 0x01;		/* Select watchdog osc */

		  //125us is one clock at 8khz
		  LPC_WWDT->TC = timeout_us / 125;	/* once WDEN is set, the WDT will start after feeding */


		  LPC_WWDT->MOD = WDEN | WDRESET;

		  LPC_WWDT->FEED = 0xAA;		/* Feeding sequence */
		  LPC_WWDT->FEED = 0x55;

	}

	static void setTC(uint32_t tc) {
		LPC_WWDT->TC = tc;
	}


	static void feed() {
		  LPC_WWDT->FEED = 0xAA;		/* Feeding sequence */
		  LPC_WWDT->FEED = 0x55;
	}



};



}


}


#endif /* WATCHDOG_HPP_ */
