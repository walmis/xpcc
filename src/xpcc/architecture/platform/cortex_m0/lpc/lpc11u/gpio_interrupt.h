/*
 * gpio_interrupt.h
 *
 *  Created on: Feb 20, 2013
 *      Author: walmis
 */

#ifndef GPIO_INTERRUPT_H_
#define GPIO_INTERRUPT_H_

#include <xpcc/architecture.hpp>
#include <xpcc/debug.hpp>

namespace xpcc {

enum class IntSense {
	EDGE = 0,
	LEVEL = 1
};

enum class IntEdge {
	SINGLE = 0,
	DOUBLE = 1
};

enum class IntEvent {
	RISING_EDGE = 0,
	FALLING_EDGE = 1,
	LEVEL_HIGH = 0,
	LEVEL_LOW = 1
};

class GpioInterrupt {
public:
	// call from interrupt context, returns true if interrupt source
	// contains all provided predicates
	static ALWAYS_INLINE
	bool checkInterrupt(int irqn, uint8_t port, uint8_t pin, IntEvent edge) {
		if(irqn >= FLEX_INT0_IRQn && irqn <= FLEX_INT7_IRQn) {
			uint8_t num = (port == 1) ? 24 + pin : pin;

			if(LPC_SYSCON->PINTSEL[irqn] == num) {

				LPC_GPIO_PIN_INT->IST |= 1<<irqn;
				return true;
			}

		}

		return false;
	}

	static void enableGlobalInterrupts() {
		for(int pinNum = 0; pinNum < 8; pinNum++) {
			if(LPC_SYSCON->PINTSEL[pinNum] != 0) {
				//XPCC_LOG_DEBUG .printf("gpio irq en %d\n", pinNum);
				NVIC_EnableIRQ((IRQn_Type)(FLEX_INT0_IRQn + pinNum));

			}
		}
	}

	static void disableGlobalInterrupts() {
		for(int pinNum = 0; pinNum < 8; pinNum++) {
			NVIC_DisableIRQ((IRQn_Type)(FLEX_INT0_IRQn + pinNum));
		}
	}

	static ALWAYS_INLINE
	void enableInterrupt(uint32_t port, uint32_t pin, IntSense sense =
			IntSense::EDGE, IntEdge edge = IntEdge::SINGLE, IntEvent event =
			IntEvent::RISING_EDGE) {

		LPC_SYSCON->SYSAHBCLKCTRL |= (1<<19); //enable clock to GPIOINT block

		uint8_t pinIdx = 24*port + pin;
		int irqNum = 0;

		for(irqNum = 0; irqNum < 8; irqNum++) {
			if(LPC_SYSCON->PINTSEL[irqNum] == pinIdx) {
				disableInterrupt(port, pin);
				break;
			}
		}

		for(irqNum = 0; irqNum < 8; irqNum++) {
			if(LPC_SYSCON->PINTSEL[irqNum] == 0) {
				break;
			}
		}

		//XPCC_LOG_DEBUG .printf("gpio pinnum irqNum:%d pin:%d\n", irqNum, pinIdx);

		LPC_SYSCON->PINTSEL[irqNum] = pinIdx;

		if(sense == IntSense::LEVEL) {
			LPC_GPIO_PIN_INT->ISEL |= 1<<irqNum;

			if(event == IntEvent::LEVEL_HIGH) {
				LPC_GPIO_PIN_INT->IENF |= 1<<irqNum;
				LPC_GPIO_PIN_INT->IENR |= 1<<irqNum;
				LPC_GPIO_PIN_INT->SIENF |= 1<<irqNum;
			} else {
				LPC_GPIO_PIN_INT->IENF &= ~(1<<irqNum);
				LPC_GPIO_PIN_INT->IENR |= 1<<irqNum;
				LPC_GPIO_PIN_INT->CIENF |= 1<<irqNum;
			}

		} else {
			LPC_GPIO_PIN_INT->ISEL &= ~(1<<irqNum);

			if(edge == IntEdge::DOUBLE) {
				//XPCC_LOG_DEBUG .printf("double edge\n");
				LPC_GPIO_PIN_INT->IENR |= 1<<irqNum;
				LPC_GPIO_PIN_INT->IENF |= 1<<irqNum;
			} else {
				if(event == IntEvent::RISING_EDGE) {
					LPC_GPIO_PIN_INT->IENR |= 1<<irqNum;
					//XPCC_LOG_DEBUG .printf("rising enable %x\n", LPC_GPIO_PIN_INT->IENR);
				} else {
					XPCC_LOG_DEBUG .printf("falling enable\n");
					LPC_GPIO_PIN_INT->IENF |= 1<<irqNum;
				}
			}
		}

	}

	static ALWAYS_INLINE
	void disableInterrupt(uint8_t port, uint8_t pin) {

		uint8_t PinIdx = 24*port + pin;
		for(int irqNum = 0; irqNum < 8; irqNum++) {

			if(LPC_SYSCON->PINTSEL[irqNum] == PinIdx) {
				NVIC_DisableIRQ((IRQn_Type)(FLEX_INT0_IRQn + irqNum));

				LPC_SYSCON->PINTSEL[irqNum] = 0;

				LPC_GPIO_PIN_INT->IENR &= ~(1<<irqNum);
				LPC_GPIO_PIN_INT->IENF &= ~(1<<irqNum);
				LPC_GPIO_PIN_INT->ISEL &= ~(1<<irqNum);

				return;
			}
		}
	}



};

} /* namespace xpcc */
#endif /* GPIO_INTERRUPT_H_ */
