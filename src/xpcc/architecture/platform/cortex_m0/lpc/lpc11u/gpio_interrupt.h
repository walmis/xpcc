/*
 * gpio_interrupt.h
 *
 *  Created on: Feb 20, 2013
 *      Author: walmis
 */

#pragma once

#include <xpcc/architecture.hpp>
#include <xpcc/processing.hpp>

namespace xpcc {

enum IntEdge {
	RISING_EDGE = 1,
	FALLING_EDGE = 2,
	LEVEL_HIGH = 4,
	LEVEL_LOW = 8
};

class GpioInt {
public:
	GpioInt(uint8_t port, uint8_t pin, xpcc::function<void()> fn,
			IntEdge edges = IntEdge::RISING_EDGE);
	~GpioInt();

	static OwnPtr<GpioInt> attach(uint8_t port, uint8_t pin, xpcc::function<void()> fn,
			IntEdge edges = IntEdge::RISING_EDGE);

	static IntEdge currentEdge();
	static uint8_t currentPort();
	static uint8_t currentPin();

	void setCallback(xpcc::function<void()> fn) {
		func = fn;
	}

	static void unmaskInterrupts() {
		for(int pinNum = 0; pinNum < 8; pinNum++) {
			if(LPC_SYSCON->PINTSEL[pinNum] != 0) {
				//XPCC_LOG_DEBUG .printf("gpio irq en %d\n", pinNum);
				NVIC_EnableIRQ((IRQn_Type)(FLEX_INT0_IRQn + pinNum));

			}
		}
	}

	static void maskInterrupts() {
		for(int pinNum = 0; pinNum < 8; pinNum++) {
			NVIC_DisableIRQ((IRQn_Type)(FLEX_INT0_IRQn + pinNum));
		}
	}

	static void _handleFLEXInt(int n);

private:
	uint8_t port;
	uint8_t pin;
	uint8_t edges;
	xpcc::function<void()> func;
	GpioInt* next = 0;
};

namespace lpc11u {
class GpioInterrupt {
public:
	static void enableInterrupt(uint32_t port, uint32_t pin, IntEdge edge = IntEdge::RISING_EDGE);
	static void disableInterrupt(uint8_t port, uint8_t pin);
};
}
} /* namespace xpcc */
