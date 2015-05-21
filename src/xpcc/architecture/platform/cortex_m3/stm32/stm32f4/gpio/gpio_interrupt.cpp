/*
 * gpio_interrupt.cpp
 *
 *  Created on: Apr 24, 2015
 *      Author: walmis
 */

#include <xpcc/container/linked_list.hpp>
#include <xpcc/processing/function.hpp>
#include "../../device.hpp"
#include "gpio_interrupt.hpp"

namespace xpcc {


struct Entry {
	uint8_t port;
	uint8_t pin;
	uint8_t edges;
	xpcc::function<void()> func;
};

static LinkedList<Entry> handlers;

static volatile uint8_t _currentEdge;
static volatile uint8_t _currentPort;
static volatile uint8_t _currentPin;

uint8_t port_bits[4] = {0, 4, 8, 12};


IntEdge GpioInt::currentEdge() {
	return (IntEdge)_currentEdge;
}

uint8_t GpioInt::currentPort() {
	return _currentPort;
}

uint8_t GpioInt::currentPin() {
	return _currentPin;
}

bool xpcc::GpioInt::attach(uint8_t port, uint8_t pin, xpcc::function<void()> fn,
		IntEdge edges) {

	port &= 0x0F;

	SYSCFG->EXTICR[pin / 4] &= ~(0x0F<<((pin & 3)*4));
	SYSCFG->EXTICR[pin / 4] |= (port<<((pin & 3)*4));

	if(IntEdge::RISING_EDGE & edges) {
		EXTI->RTSR |= (1<<pin);
	} else {
		EXTI->RTSR &= ~(1<<pin);
	}
	if(IntEdge::FALLING_EDGE & edges) {
		EXTI->FTSR |= (1<<pin);
	} else {
		EXTI->FTSR &= ~(1<<pin);
	}

	Entry e;
	e.pin = pin;
	e.port = port;
	e.func = fn;
	e.edges = (uint8_t) edges;

	handlers.append(e);

	EXTI->IMR |= (1<<pin);

	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI4_IRQn);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	return true;
}

static void handleEXTI() {
	uint32_t flags = EXTI->PR;
	if(!flags) return;

	for(auto e : handlers) {

		if(flags & (1<<e.pin)) {
			_currentPin = e.pin;
			_currentPort = e.port;
			_currentEdge = 0; //unknown

			if(e.func) e.func();

			EXTI->PR |= (1<<e.pin);
		}
	}

}

extern "C"
void EXTI0_IRQHandler() {
	handleEXTI();
}

extern "C"
void EXTI1_IRQHandler() {
	handleEXTI();
}

extern "C"
void EXTI2_IRQHandler() {
	handleEXTI();
}

extern "C"
void EXTI3_IRQHandler() {
	handleEXTI();
}

extern "C"
void EXTI4_IRQHandler() {
	handleEXTI();
}

extern "C"
void EXTI9_5_IRQHandler() {
	handleEXTI();
}

extern "C"
void EXTI15_10_IRQHandler() {
	handleEXTI();
}

}


