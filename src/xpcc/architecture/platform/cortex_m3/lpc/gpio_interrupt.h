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

class GpioInterrupt {
public:

	static ALWAYS_INLINE
	bool checkInterrupt(int irqn, uint8_t port, uint8_t pin, IntEvent edge) {
		bool result = false;
		if(irqn == EINT3_IRQn) {
			if(port == 0 && (LPC_GPIOINT->IntStatus & 1)) {

				if(LPC_GPIOINT->IO0IntStatR & (1<<pin)) {
					if(edge == IntEvent::RISING_EDGE)
						result = true;

				}
				if(LPC_GPIOINT->IO0IntStatF & (1<<pin)) {
					if(edge == IntEvent::FALLING_EDGE)
						result = true;
				}

				LPC_GPIOINT->IO0IntClr = 1<<pin;

			}
			else if(port == 2 && (LPC_GPIOINT->IntStatus & 4)) {

					if(LPC_GPIOINT->IO2IntStatR & (1<<pin)) {
						if(edge == IntEvent::RISING_EDGE)
							result = true;
					}
					if(LPC_GPIOINT->IO2IntStatF & (1<<pin)) {
						if(edge == IntEvent::FALLING_EDGE)
							result = true;
					}
					LPC_GPIOINT->IO2IntClr = (1<<pin);
			}
		}
		return result;
	}


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

	static ALWAYS_INLINE
	void disableInterrupt(uint8_t port, uint8_t pin) {
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

} /* namespace xpcc */
#endif /* GPIO_INTERRUPT_H_ */
