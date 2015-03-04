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
	xpcc::function<void()> func;
};

static LinkedList<Entry> handlers;

static uint8_t _currentEdge;
static uint8_t _currentPort;
static uint8_t _currentPin;

void GpioInt::attach(uint8_t port, uint8_t pin,
		xpcc::function<void()> fn, IntEdge edges) {

	Entry e;
	e.port = port;
	e.pin = pin;
	e.edges = edges;
	e.func = fn;

	handlers.append(e);

	GpioInt::enableInterrupt(port, pin, edges);

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
    uint32_t rise0 = LPC_GPIOINT->IO0IntStatR;
    uint32_t fall0 = LPC_GPIOINT->IO0IntStatF;
    uint32_t rise2 = LPC_GPIOINT->IO2IntStatR;
    uint32_t fall2 = LPC_GPIOINT->IO2IntStatF;

	for(Entry& e : handlers ) {

		if(e.port == 0) {
			_currentPort = e.port;
			_currentPin = e.pin;

			uint32_t mask = (1<<e.pin);

			if((rise0|fall0) & mask) {
				//clear interrupt flag immediately
				LPC_GPIOINT->IO0IntClr = mask;
			}

			if(rise0 & mask) {
				if(e.edges & IntEdge::RISING_EDGE) {
					_currentEdge = IntEdge::RISING_EDGE;
					e.func(); //call handler
					rise0 &= ~mask; //clear pin
				}

			}
			if(fall0 & mask) {
				if(e.edges & IntEdge::FALLING_EDGE) {
					_currentEdge = IntEdge::FALLING_EDGE;
					e.func(); //call handler
					fall0 &= ~mask; //clear pin
				}
			}


		}
		else if(e.port == 2) {
			_currentPort = e.port;
			_currentPin = e.pin;

			uint32_t mask = (1<<e.pin);

			if((rise2|fall2) & mask) {
				//clear interrupt flag immediately
				LPC_GPIOINT->IO2IntClr = mask;
			}

			if(rise2 & mask) {
				if(e.edges & IntEdge::RISING_EDGE) {
					_currentEdge = IntEdge::RISING_EDGE;
					e.func();
					rise2 &= ~mask; //clear pin
				}
			}
			if(fall2 & mask) {
				if(e.edges & IntEdge::FALLING_EDGE) {
					_currentEdge = IntEdge::FALLING_EDGE;
					e.func();
					fall2 &= ~mask; //clear pin
				}
			}

		}

	}

	_currentEdge = 0;
	_currentPort = 0;
	_currentPin = 0;

	uint32_t p0 = (rise0 | fall0);
	uint32_t p2 = (rise2 | fall2);
	//if there still are some unhandled pins, run the default irq handler
	if(p0 | p2) {
		int irqn = __get_IPSR() - 16;
		xpcc::TickerTask::interrupt(irqn);

		//clear any unhandled interrupts
		LPC_GPIOINT->IO0IntClr = p0;
		LPC_GPIOINT->IO2IntClr = p2;
	}
}

}
