/*
 * gpio_interrupt.h
 *
 *  Created on: Feb 20, 2013
 *      Author: walmis
 */

#ifndef GPIO_INTERRUPT_H_
#define GPIO_INTERRUPT_H_

#include <xpcc/architecture.hpp>


namespace xpcc {
namespace lpc17 {

enum class IntSense {
	EDGE = 0,
};

enum class IntEdge {
	SINGLE = 0,
	DOUBLE = 1
};

enum class IntEvent {
	RISING_EDGE = 0,
	FALLING_EDGE = 1
};

typedef void (*GpioIntHandler)(uint8_t pin, IntEvent edge);

class GpioInterrupt {
public:
	static void registerPortHandler(uint8_t port, GpioIntHandler func);


	static void enableGlobalInterrupts() {
		NVIC_EnableIRQ(EINT3_IRQn);
	}

	static void disableGlobalInterrupts() {
		NVIC_DisableIRQ(EINT3_IRQn);
	}

	static ALWAYS_INLINE
	void enableInterrupt(uint32_t port, uint32_t pin, IntSense sense =
			IntSense::EDGE, IntEdge edge = IntEdge::SINGLE, IntEvent event =
			IntEvent::RISING_EDGE) {

		uint32_t mask = 1<<pin;
		if(edge == IntEdge::SINGLE) {
			if((port == 0)&&(event==IntEvent::RISING_EDGE))
				LPC_GPIOINT->IO0IntEnR |= mask;
			else if ((port == 2)&&(event==IntEvent::RISING_EDGE))
				LPC_GPIOINT->IO2IntEnR |= mask;
			else if ((port == 0)&&(event==IntEvent::FALLING_EDGE))
				LPC_GPIOINT->IO0IntEnF |= mask;
			else if ((port == 2)&&(event==IntEvent::FALLING_EDGE))
				LPC_GPIOINT->IO2IntEnF |= mask;
		} else {

			if(port == 0) {
				LPC_GPIOINT->IO0IntEnR |= mask;
				LPC_GPIOINT->IO0IntEnF |= mask;
			} else if(port == 2) {
				LPC_GPIOINT->IO2IntEnF |= mask;
				LPC_GPIOINT->IO2IntEnR |= mask;
			}

		}
	}

	static ALWAYS_INLINE void disableInterrupt(uint8_t port, uint8_t pin) {
		uint32_t mask = 1<<pin;
		if(port == 0) {
			LPC_GPIOINT->IO0IntEnR &= ~mask;
			LPC_GPIOINT->IO0IntEnF &= ~mask;
		}
		else if (port == 2) {
			LPC_GPIOINT->IO2IntEnR &= ~mask;
			LPC_GPIOINT->IO2IntEnF &= ~mask;
		}
	}
};

} /* namespace lpc */
} /* namespace xpcc */
#endif /* GPIO_INTERRUPT_H_ */
