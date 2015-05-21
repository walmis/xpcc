/*
 * rcc.hpp
 *
 *  Created on: Apr 8, 2015
 *      Author: walmis
 */

#ifndef SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_RCC_HPP_
#define SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_RCC_HPP_

#include "../device.hpp"

namespace xpcc {
namespace stm32 {

static constexpr uint8_t APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};

class Clocks {
public:

	static uint32_t getHCLKFrequency() {
		uint32_t tmp;
		tmp = RCC->CFGR & RCC_CFGR_HPRE;
		tmp = tmp >> 4;
		uint8_t presc = APBAHBPrescTable[tmp];
		/* HCLK clock frequency */
		return SystemCoreClock >> presc;

	}

	static uint32_t getPCLK1Frequency() {
		uint32_t tmp;
		tmp = RCC->CFGR & RCC_CFGR_PPRE1;
		tmp = tmp >> 10;
		uint8_t presc = APBAHBPrescTable[tmp];
		/* PCLK1 clock frequency */
		return getHCLKFrequency() >> presc;

	}

	static uint32_t getPCLK2Frequency() {
		uint32_t tmp = RCC->CFGR & RCC_CFGR_PPRE2;
		tmp = tmp >> 13;
		uint8_t presc = APBAHBPrescTable[tmp];
		/* PCLK2 clock frequency */
		return getHCLKFrequency() >> presc;

	}


};


}
}


#endif /* SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_RCC_HPP_ */
