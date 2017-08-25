/*
 * gpio_interrupt.cpp
 *
 *  Created on: Apr 24, 2015
 *      Author: walmis
 */

#include <xpcc/container/linked_list.hpp>
#include <xpcc/processing/function.hpp>
#include <xpcc/processing/rtos_abstraction.hpp>
#include "../../device.hpp"
#include "gpio_interrupt.hpp"
#include "gpio.hpp"
namespace xpcc {

static GpioInt* handlers;

static volatile uint8_t _currentEdge;
static volatile uint8_t _currentPort;
static volatile uint8_t _currentPin;

const uint8_t port_bits[4] = {0, 4, 8, 12};


IntEdge GpioInt::currentEdge() {
	return (IntEdge)_currentEdge;
}

uint8_t GpioInt::currentPort() {
	return _currentPort;
}

uint8_t GpioInt::currentPin() {
	return _currentPin;
}

xpcc::OwnPtr<GpioInt>  xpcc::GpioInt::attach(uint8_t port, uint8_t pin, xpcc::function<void()> fn,
		IntEdge edges) {

	GpioInt *e = new GpioInt(port, pin, fn, edges);

	e->enable();

	return e;
}

void xpcc::GpioInt::setEdges(IntEdge edges) {
	this->edges = (uint8_t)edges;

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
}

xpcc::GpioInt::GpioInt(uint8_t port, uint8_t pin, xpcc::function<void()> callback, IntEdge edges) {
	this->pin = pin;
	this->port = port;
	this->func = callback;
	this->edges = (uint8_t) edges;

	port &= 0x0F;

	SYSCFG->EXTICR[pin / 4] &= ~(0x0F<<((pin & 3)*4));
	SYSCFG->EXTICR[pin / 4] |= (port<<((pin & 3)*4));

	setEdges(edges);

	//append handler to the chain
	if(!handlers) {
		handlers = this;
	} else {
		GpioInt* n = handlers;
		while(n->next) {
			n = n->next;
		}
		n->next = this;
	}

	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI4_IRQn);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	NVIC_EnableIRQ(EXTI15_10_IRQn);

}

xpcc::GpioInt::~GpioInt() {
	__disable_irq();

	disable(); //mask irq

	GpioInt* e = handlers;
	GpioInt* prev = 0;

	if(this == handlers) {
		handlers = 0;
	} else {
		while(e) {
			if(e == this) {
				if(prev) {
					prev->next = e->next;
					break;
				}
			}
			prev = e;
			e = e->next;
		}
	}

	__enable_irq();
}

void xpcc::GpioInt::enable() {
		EXTI->IMR |= (1<<pin); //interrupt mask register
}

void xpcc::GpioInt::disable() {
		EXTI->IMR &= ~(1<<pin); //interrupt mask register
}

void xpcc::GpioInt::_handleInterrupt() {
	uint32_t flags = EXTI->PR;
	if(!flags) return;

	EXTI->PR = flags; //clear pending interrupts immediately

	GpioInt* e = handlers;

	while(e) {
		if(flags & (1<<e->pin)) {
			_currentPin = e->pin;
			_currentPort = e->port;
			_currentEdge = xpcc::stm32::GPIO::read(_currentPort, _currentPin); 

			e->func();
		}

		e = e->next;
	}
}

extern "C"
void EXTI0_IRQHandler() {
	IRQWrapper w;
	xpcc::GpioInt::_handleInterrupt();
}

extern "C"
void EXTI1_IRQHandler() {
	IRQWrapper w;
	xpcc::GpioInt::_handleInterrupt();
}

extern "C"
void EXTI2_IRQHandler() {
	IRQWrapper w;
	xpcc::GpioInt::_handleInterrupt();
}

extern "C"
void EXTI3_IRQHandler() {
	IRQWrapper w;
	xpcc::GpioInt::_handleInterrupt();
}

extern "C"
void EXTI4_IRQHandler() {
	IRQWrapper w;
	xpcc::GpioInt::_handleInterrupt();
}

extern "C"
void EXTI9_5_IRQHandler() {
	IRQWrapper w;
	xpcc::GpioInt::_handleInterrupt();
}

extern "C"
void EXTI15_10_IRQHandler() {
	IRQWrapper w;
	xpcc::GpioInt::_handleInterrupt();
}

}
