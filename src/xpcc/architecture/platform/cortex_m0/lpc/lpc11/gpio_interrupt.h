/*
 * gpio_interrupt.h
 *
 *  Created on: Feb 20, 2013
 *      Author: walmis
 */

#ifndef GPIO_INTERRUPT_H_
#define GPIO_INTERRUPT_H_

#include <xpcc/architecture.hpp>
#include <xpcc/processing/function.hpp>
#include <lpc11xx/cmsis/LPC11xx.h>

namespace xpcc {

enum class IntSense {
	EDGE = 0,
	LEVEL = 1
};

enum class IntEvent {
	RISING_EDGE = 0,
	FALLING_EDGE = 1
};

enum class IntEdge {
	RISING_EDGE = 1,
	FALLING_EDGE = 2,
	LEVEL_HIGH = 4,
	LEVEL_LOW = 8
};
ENUM_CLASS_FLAG(IntEdge);

class GpioInt {
public:
	struct Entry {
		uint8_t port;
		uint8_t pin;
		uint8_t edges;
		xpcc::function<void()> func;
		Entry* next = 0;
	};

	static ALWAYS_INLINE
	void enableInterrupt(uint32_t port, uint32_t pin, IntEdge edge = IntEdge::RISING_EDGE) {

		uint32_t pin_msk = 1 << pin;
		switch (port) {
		case 0:
			enableInt(LPC_GPIO0, pin_msk, edge);
			break;
		case 1:
			enableInt(LPC_GPIO1, pin_msk, edge);
			break;
		case 2:
			enableInt(LPC_GPIO2, pin_msk, edge);
			break;
		case 3:
			enableInt(LPC_GPIO3, pin_msk, edge);
			break;
		default:
			break;
		}
	}

	static bool attach(uint8_t port, uint8_t pin, xpcc::function<void()> fn,
			IntEdge edges = IntEdge::RISING_EDGE);

	static IntEdge currentEdge();
	static uint8_t currentPort();
	static uint8_t currentPin();

	GpioInt(uint8_t port, uint8_t pin, IntEdge edges = IntEdge::RISING_EDGE, xpcc::function<void()> callback = 0);

	void enable();
	void disable();

	void setCallback(xpcc::function<void()> fn) {
		int_info.func = fn;
	}

	void setIrqPriority(uint8_t irq_prio) {
		switch(int_info.port) {
		case 0:
			NVIC_SetPriority(EINT0_IRQn, irq_prio);
			break;
		case 1:
			NVIC_SetPriority(EINT1_IRQn, irq_prio);
			break;
		case 2:
			NVIC_SetPriority(EINT2_IRQn, irq_prio);
			break;
		case 3:
			NVIC_SetPriority(EINT3_IRQn, irq_prio);
			break;
		}
	}

	static void enableInterrupts() {
		NVIC_EnableIRQ(EINT0_IRQn);
		NVIC_EnableIRQ(EINT1_IRQn);
		NVIC_EnableIRQ(EINT2_IRQn);
		NVIC_EnableIRQ(EINT3_IRQn);
	}

	static void disableInterrupts() {
		NVIC_DisableIRQ(EINT0_IRQn);
		NVIC_DisableIRQ(EINT1_IRQn);
		NVIC_DisableIRQ(EINT2_IRQn);
		NVIC_DisableIRQ(EINT3_IRQn);
	}

private:

	static ALWAYS_INLINE
	void enableInt(LPC_GPIO_TypeDef* port, uint32_t pin_msk, IntEdge edge) {
		port->IE |= pin_msk;

		if (edge & (IntEdge::RISING_EDGE|IntEdge::FALLING_EDGE)) {
			port ->IS &= ~(pin_msk);
			/* edge or double only applies when sense is 0(edge trigger). */
			if (edge == (IntEdge::FALLING_EDGE|IntEdge::RISING_EDGE))
				port ->IBE |= (pin_msk);
			else
				port ->IBE &= ~(pin_msk);

		} else {
			port ->IS |= (pin_msk);
		}

		if (edge & (IntEdge::FALLING_EDGE|IntEdge::LEVEL_LOW))
			port ->IEV &= ~(pin_msk);
		else
			port ->IEV |= (pin_msk);
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

	Entry int_info;
};



class GpioInterrupt {
public:
	// call from interrupt context, returns true if interrupt source
	// contains all provided predicates
	static ALWAYS_INLINE
	bool checkInterrupt(int irqn, uint8_t port, uint8_t pin, IntEvent edge) {

		if(port == 0 && irqn == EINT0_IRQn) {
			if(LPC_GPIO0->MIS & (1 << pin)) {
				LPC_GPIO0->IC |= (1<<pin);
				return true;
			}
		} else if(port == 1 && irqn ==  EINT1_IRQn) {
			if(LPC_GPIO1->MIS & (1 << pin)) {
				LPC_GPIO1->IC |= (1<<pin);
				return true;
			}
		} else if(port == 2 && irqn == EINT2_IRQn) {
			if(LPC_GPIO2->MIS & (1 << pin)) {
				LPC_GPIO2->IC |= (1<<pin);
				return true;
			}
		} else if(port == 3 && irqn == EINT3_IRQn) {
			if(LPC_GPIO3->MIS & (1 << pin)) {
				LPC_GPIO3->IC |= (1<<pin);
				return true;
			}
		}
		return false;
	}









};

} /* namespace xpcc */
#endif /* GPIO_INTERRUPT_H_ */
