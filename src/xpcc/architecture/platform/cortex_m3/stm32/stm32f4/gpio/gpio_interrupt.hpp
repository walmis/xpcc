/*
 * gpio_interrupt.hpp
 *
 *  Created on: Apr 24, 2015
 *      Author: walmis
 */

#ifndef SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_GPIO_INTERRUPT_HPP_
#define SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_GPIO_INTERRUPT_HPP_

#include <xpcc/architecture/utils.hpp>
#include <xpcc/processing/function.hpp>
#include <xpcc/processing/own_ptr.hpp>

namespace xpcc {

enum class IntEdge {
	RISING_EDGE = 1<<0,
	FALLING_EDGE = 1<<1
};

class GpioInt {
public:
	//returns a newly allocated GpioInt object
	static OwnPtr<GpioInt> attach(uint8_t port, uint8_t pin, xpcc::function<void()> fn,
			IntEdge edges = IntEdge::RISING_EDGE);

	GpioInt(uint8_t port, uint8_t pin,
		xpcc::function<void()> callback = 0, IntEdge edges = IntEdge::RISING_EDGE);

	~GpioInt();

	void enable();
	void disable();

	void setCallback(xpcc::function<void()> fn) { func = fn; }
	void setEdges(IntEdge edges);

	static IntEdge currentEdge();
	static uint8_t currentPort();
	static uint8_t currentPin();

	static void _handleInterrupt();

private:
	uint8_t port;
	uint8_t pin;
	uint8_t edges;
	xpcc::function<void()> func;
	GpioInt* next = 0;
};

ENUM_CLASS_FLAG(IntEdge);

}



#endif /* SRC_XPCC_ARCHITECTURE_PLATFORM_CORTEX_M3_STM32_STM32F4_GPIO_INTERRUPT_HPP_ */
