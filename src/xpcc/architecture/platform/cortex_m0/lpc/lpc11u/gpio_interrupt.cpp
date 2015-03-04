/*
 * gpio_interrupt.cpp
 *
 *  Created on: Mar 3, 2015
 *      Author: walmis
 */

#include "gpio_interrupt.h"
#include <xpcc/container/linked_list.hpp>

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

	lpc11u::GpioInterrupt::enableInterrupt(port, pin, edges);

}

static void handleFLEXInt(uint8_t idx) {
	for(auto &handler : handlers) {
		uint8_t num = (handler.port == 1) ? 24 + handler.pin : handler.pin;

		if(LPC_SYSCON->PINTSEL[idx] == num) {

			_currentPort = handler.port;
			_currentPin = handler.pin;
			_currentEdge = 0;

			if(LPC_GPIO_PIN_INT->FALL & (1<<idx)) {
				_currentEdge = IntEdge::FALLING_EDGE;
			}
			if(LPC_GPIO_PIN_INT->RISE & (1<<idx)) {
				_currentEdge = IntEdge::RISING_EDGE;
			}

			LPC_GPIO_PIN_INT->IST |= (1<<idx); //clear int pending

			handler.func();

			break;
		}
	}
}

extern "C"
void FLEX_INT0_IRQHandler() {
	handleFLEXInt(0);
}
extern "C"
void FLEX_INT1_IRQHandler() {
	handleFLEXInt(1);
}
extern "C"
void FLEX_INT2_IRQHandler() {
	handleFLEXInt(2);
}
extern "C"
void FLEX_INT3_IRQHandler() {
	handleFLEXInt(3);
}
extern "C"
void FLEX_INT4_IRQHandler() {
	handleFLEXInt(4);
}
extern "C"
void FLEX_INT5_IRQHandler() {
	handleFLEXInt(5);
}
extern "C"
void FLEX_INT6_IRQHandler() {
	handleFLEXInt(6);
}
extern "C"
void FLEX_INT7_IRQHandler() {
	handleFLEXInt(7);
}

}
