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

namespace lpc11 {

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

		  LPC_SYSCON->WDTCLKSEL = 0x02;		/* Select watchdog osc */
		  LPC_SYSCON->WDTCLKUEN = 0x01;		/* Update clock */
		  LPC_SYSCON->WDTCLKUEN = 0x00;		/* Toggle update register once */
		  LPC_SYSCON->WDTCLKUEN = 0x01;
		  while ( !(LPC_SYSCON->WDTCLKUEN & 0x01) );		/* Wait until updated */
		  LPC_SYSCON->WDTCLKDIV = 1;		/* Divided by 1 */

		  //125us is one clock at 8khz
		  LPC_WDT->TC = timeout_us / 125;	/* once WDEN is set, the WDT will start after feeding */


		  LPC_WDT->MOD = WDEN | WDRESET;

		  LPC_WDT->FEED = 0xAA;		/* Feeding sequence */
		  LPC_WDT->FEED = 0x55;

	}


	static void feed() {
		  LPC_WDT->FEED = 0xAA;		/* Feeding sequence */
		  LPC_WDT->FEED = 0x55;
	}



};



}


}


#endif /* WATCHDOG_HPP_ */
