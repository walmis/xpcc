/*
 * gpio_interrupt.hpp
 *
 *  Created on: Apr 24, 2015
 *      Author: walmis
 */

#ifndef SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_GPIO_INTERRUPT_HPP_
#define SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_GPIO_INTERRUPT_HPP_

namespace xpcc {

enum IntEdge {
	RISING_EDGE = 1,
	FALLING_EDGE = 2
};

class GpioInt {
public:

	static void attach(uint8_t port, uint8_t pin, xpcc::function<void()> fn,
			IntEdge edges = IntEdge::RISING_EDGE);


};

}



#endif /* SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_GPIO_INTERRUPT_HPP_ */
