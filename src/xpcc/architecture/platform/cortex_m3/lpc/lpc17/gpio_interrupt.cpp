/*
 * gpio_interrupt.cpp
 *
 *  Created on: Sep 20, 2014
 *      Author: walmis
 */


#include "gpio_interrupt.h"
#include <xpcc/container/linked_list.hpp>
#include <stdio.h>

namespace xpcc {

struct Entry {
	uint8_t port;
	uint8_t pin;
	uint8_t edges;
	std::function<void()> func;
};

static LinkedList<Entry> handlers;

static uint8_t _currentEdge;
static uint8_t _currentPort;
static uint8_t _currentPin;

void GpioInt::attach(uint8_t port, uint8_t pin,
		std::function<void()> fn, IntEdge edges) {

	Entry e;
	e.port = port;
	e.pin = pin;
	e.edges = edges;
	e.func = fn;

	GpioInt::enableInterrupt(port, pin, edges);

	handlers.append(e);

}

IntEdge GpioInt::currentEdge() {
	return (IntEdge)_currentEdge;
}

uint8_t GpioInt::currentPort() {
	return _currentPort;
}

uint8_t GpioInt::currentPin() {
	return _currentPin;
}

extern "C" void EINT3_IRQHandler() {
	uint32_t i0clrMask = 0;
	uint32_t i2clrMask = 0;
	bool result;

    uint32_t rise0 = LPC_GPIOINT->IO0IntStatR;
    uint32_t fall0 = LPC_GPIOINT->IO0IntStatF;
    uint32_t rise2 = LPC_GPIOINT->IO2IntStatR;
    uint32_t fall2 = LPC_GPIOINT->IO2IntStatF;

	for(Entry& e : handlers ) {

		if(e.port == 0) {
			_currentPort = e.port;
			_currentPin = e.pin;

			uint32_t mask = (1<<e.pin);
			result = false;

			if(rise0 & mask) {
				if(e.edges & IntEdge::RISING_EDGE) {
					_currentEdge = IntEdge::RISING_EDGE;
					e.func();
					result = true;
				}

			}
			if(fall0 & mask) {
				if(e.edges & IntEdge::FALLING_EDGE) {
					_currentEdge = IntEdge::FALLING_EDGE;
					e.func();
					result = true;
				}
			}
			if(result)
				i0clrMask |= mask;

		}
		else if(e.port == 2) {
			_currentPort = e.port;
			_currentPin = e.pin;

			uint32_t mask = (1<<e.pin);
			result = false;
			if(rise2 & mask) {
				if(e.edges & IntEdge::RISING_EDGE) {
					_currentEdge = IntEdge::RISING_EDGE;
					e.func();
					result = true;
				}
			}
			if(fall2 & mask) {
				if(e.edges & IntEdge::FALLING_EDGE) {
					_currentEdge = IntEdge::FALLING_EDGE;
					e.func();
					result = true;
				}
			}
			if(result)
				i2clrMask |= mask;
		}

	}

	_currentEdge = 0;
	_currentPort = 0;
	_currentPin = 0;

	LPC_GPIOINT->IO0IntClr = i0clrMask;
	LPC_GPIOINT->IO2IntClr = i2clrMask;

	//if there still are some unhandled pins, run the default irq handler
	if(LPC_GPIOINT->IO0IntStatF ||  LPC_GPIOINT->IO0IntStatR ||
			LPC_GPIOINT->IO2IntStatF || LPC_GPIOINT->IO0IntStatR) {
		int irqn = __get_IPSR() - 16;
		xpcc::TickerTask::interrupt(irqn);
	}

	//clear any unhandled interrupts
	LPC_GPIOINT->IO0IntClr = 0xFFFFFFFF;
	LPC_GPIOINT->IO2IntClr = 0xFFFFFFFF;

}

}
