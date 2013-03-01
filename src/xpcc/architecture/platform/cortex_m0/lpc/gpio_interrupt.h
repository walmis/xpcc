/*
 * gpio_interrupt.h
 *
 *  Created on: Feb 20, 2013
 *      Author: walmis
 */

#ifndef GPIO_INTERRUPT_H_
#define GPIO_INTERRUPT_H_

#include <xpcc/architecture.hpp>
#include <LPC11xx.h>

namespace xpcc {
namespace lpc {

typedef void (*GpioIntHandler)(uint8_t pin);

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
	FALLING_EDGE = 1
};



class GpioInterrupt {
public:
	static void registerPortHandler(uint8_t port, GpioIntHandler func);


	static void enableGlobalInterrupts() {
		NVIC_EnableIRQ(EINT0_IRQn);
		NVIC_EnableIRQ(EINT1_IRQn);
		NVIC_EnableIRQ(EINT2_IRQn);
		NVIC_EnableIRQ(EINT3_IRQn);
	}

	static void disableGlobalInterrupts() {
		NVIC_DisableIRQ(EINT0_IRQn);
		NVIC_DisableIRQ(EINT1_IRQn);
		NVIC_DisableIRQ(EINT2_IRQn);
		NVIC_DisableIRQ(EINT3_IRQn);
	}

	static ALWAYS_INLINE
	void enableInterrupt(uint32_t port, uint32_t pin, IntSense sense =
			IntSense::EDGE, IntEdge edge = IntEdge::SINGLE, IntEvent event =
			IntEvent::RISING_EDGE) {

		uint32_t pin_msk = 1 << pin;
		switch (port) {
		case 0:
			LPC_GPIO0->IE |= pin_msk;

			if (sense == IntSense::EDGE) {
				LPC_GPIO0 ->IS &= ~(pin_msk);
				/* edge or double only applies when sense is 0(edge trigger). */
				if (edge == IntEdge::SINGLE)
					LPC_GPIO0 ->IBE &= ~(pin_msk);
				else
					LPC_GPIO0 ->IBE |= (pin_msk);
			} else
				LPC_GPIO0 ->IS |= (pin_msk);
			if (event == IntEvent::RISING_EDGE)
				LPC_GPIO0 ->IEV &= ~(pin_msk);
			else
				LPC_GPIO0 ->IEV |= (pin_msk);
			break;
		case 1:
			LPC_GPIO1->IE |= pin_msk;
			if (sense == IntSense::EDGE) {
				LPC_GPIO1 ->IS &= ~(pin_msk);
				/* edge or double only applies when sense is 0(edge trigger). */
				if (edge == IntEdge::SINGLE)
					LPC_GPIO1 ->IBE &= ~(pin_msk);
				else
					LPC_GPIO1 ->IBE |= (pin_msk);
			} else
				LPC_GPIO1 ->IS |= (pin_msk);
			if (event == IntEvent::RISING_EDGE)
				LPC_GPIO1 ->IEV &= ~(pin_msk);
			else
				LPC_GPIO1 ->IEV |= (pin_msk);
			break;
		case 2:
			LPC_GPIO2->IE |= pin_msk;
			if (sense == IntSense::EDGE) {
				LPC_GPIO2 ->IS &= ~(pin_msk);
				/* edge or double only applies when sense is 0(edge trigger). */
				if (edge == IntEdge::SINGLE)
					LPC_GPIO2 ->IBE &= ~(pin_msk);
				else
					LPC_GPIO2 ->IBE |= (pin_msk);
			} else
				LPC_GPIO2 ->IS |= (pin_msk);
			if (event == IntEvent::RISING_EDGE)
				LPC_GPIO2 ->IEV &= ~(pin_msk);
			else
				LPC_GPIO2 ->IEV |= (pin_msk);
			break;
		case 3:
			LPC_GPIO3->IE |= pin_msk;
			if (sense == IntSense::EDGE) {
				LPC_GPIO3 ->IS &= ~(pin_msk);
				/* edge or double only applies when sense is 0(edge trigger). */
				if (edge == IntEdge::SINGLE)
					LPC_GPIO3 ->IBE &= ~(pin_msk);
				else
					LPC_GPIO3 ->IBE |= (pin_msk);
			} else
				LPC_GPIO3 ->IS |= (pin_msk);
			if (event == IntEvent::RISING_EDGE)
				LPC_GPIO3 ->IEV &= ~(pin_msk);
			else
				LPC_GPIO3 ->IEV |= (pin_msk);
			break;
		default:
			break;
		}
	}

	static ALWAYS_INLINE void disableInterrupt(uint8_t port, uint8_t pin) {
		switch (port) {
		case 0:
			LPC_GPIO0 ->IE &= ~(0x1 << pin);
			break;
		case 1:
			LPC_GPIO1 ->IE &= ~(0x1 << pin);
			break;
		case 2:
			LPC_GPIO2 ->IE &= ~(0x1 << pin);
			break;
		case 3:
			LPC_GPIO3 ->IE &= ~(0x1 << pin);
			break;
		default:
			break;
		}
		return;
	}



};

} /* namespace lpc */
} /* namespace xpcc */
#endif /* GPIO_INTERRUPT_H_ */
