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


enum IntEdge {
	RISING_EDGE = 1,
	FALLING_EDGE = 2
};

class GpioInt {
public:

	static ALWAYS_INLINE
	bool checkInterrupt(int irqn, uint8_t port, uint8_t pin, IntEdge edges) {
		bool result = false;
		if(irqn == EINT3_IRQn) {
			if(port == 0 && (LPC_GPIOINT->IntStatus & 1)) {

				if(LPC_GPIOINT->IO0IntStatR & (1<<pin)) {
					if(edges & IntEdge::RISING_EDGE)
						result = true;

				}
				if(LPC_GPIOINT->IO0IntStatF & (1<<pin)) {
					if(edges & IntEdge::FALLING_EDGE)
						result = true;
				}
				if(result)
					LPC_GPIOINT->IO0IntClr = 1<<pin;

			}
			else if(port == 2 && (LPC_GPIOINT->IntStatus & 4)) {

					if(LPC_GPIOINT->IO2IntStatR & (1<<pin)) {
						if(edges & IntEdge::RISING_EDGE)
							result = true;
					}
					if(LPC_GPIOINT->IO2IntStatF & (1<<pin)) {
						if(edges & IntEdge::FALLING_EDGE)
							result = true;
					}
					if(result)
						LPC_GPIOINT->IO2IntClr = (1<<pin);
			}
		}
		return result;
	}


	static ALWAYS_INLINE void enableInterrupts() {
		NVIC_EnableIRQ(EINT3_IRQn);
	}

	static ALWAYS_INLINE void disableInterrupts() {
		NVIC_DisableIRQ(EINT3_IRQn);
	}

	static ALWAYS_INLINE
	void enableInterrupt(uint32_t port, uint32_t pin, IntEdge edges = IntEdge::RISING_EDGE) {

		uint32_t mask = 1<<pin;

		if((port == 0)&&(edges & IntEdge::RISING_EDGE))
			LPC_GPIOINT->IO0IntEnR |= mask;
		if ((port == 2)&&(edges & IntEdge::RISING_EDGE))
			LPC_GPIOINT->IO2IntEnR |= mask;
		if ((port == 0)&&(edges & IntEdge::FALLING_EDGE))
			LPC_GPIOINT->IO0IntEnF |= mask;
		if ((port == 2)&&(edges & IntEdge::FALLING_EDGE))
			LPC_GPIOINT->IO2IntEnF |= mask;

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

	static void attach(uint8_t port, uint8_t pin, std::function<void()> fn,
			IntEdge edges = IntEdge::RISING_EDGE);

	static IntEdge currentEdge();
	static uint8_t currentPort();
	static uint8_t currentPin();

};

} /* namespace xpcc */
#endif /* GPIO_INTERRUPT_H_ */
