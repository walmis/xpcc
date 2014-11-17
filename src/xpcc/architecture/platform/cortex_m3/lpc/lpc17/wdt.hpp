/*
 * wdt.hpp
 *
 *  Created on: Nov 17, 2014
 *      Author: walmis
 */

#ifndef SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_LPC_LPC17_WDT_HPP_
#define SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_LPC_LPC17_WDT_HPP_

#include <xpcc/architecture.hpp>

namespace xpcc {
namespace lpc17 {

class WDT {
public:
	static void init(int16_t timeout_ms) {
		LPC_WDT->WDTC = (timeout_ms*1000000UL) /
					(1000000000UL/(CLKPwr::getPCLK(CLKPwr::ClkType::WDT)/4));

		LPC_WDT->WDMOD = 0x1;

		feed();
	}

	static void feed() {
		LPC_WDT->WDFEED = 0xAA;
		LPC_WDT->WDFEED = 0x55;
	}

	static void enableIRQ() {
		NVIC_SetPriority(WDT_IRQn, 0);
		NVIC_EnableIRQ(WDT_IRQn);
	}
};

}
}



#endif /* SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_LPC_LPC17_WDT_HPP_ */
