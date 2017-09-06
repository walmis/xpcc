/*
 * gpio_interrupt.cpp
 *
 *  Created on: Mar 3, 2015
 *      Author: walmis
 */

#include "gpio_interrupt.h"

namespace xpcc {

static GpioInt* handlers;

static uint8_t _currentEdge;
static uint8_t _currentPort;
static uint8_t _currentPin;

GpioInt::GpioInt(uint8_t port, uint8_t pin,
		xpcc::function<void()> fn, IntEdge edges) :
			port(port), pin(pin), func(fn), edges(edges) {

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

	lpc11u::GpioInterrupt::enableInterrupt(port, pin, edges);
}

GpioInt::~GpioInt() {
	__disable_irq();

	lpc11u::GpioInterrupt::disableInterrupt(port, pin);

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

xpcc::OwnPtr<GpioInt> GpioInt::attach(uint8_t port, uint8_t pin,
		xpcc::function<void()> fn, IntEdge edges) {

	GpioInt *i = new GpioInt(port, pin, fn, edges);
	return i;
}

void GpioInt::_handleFLEXInt(int idx) {
	GpioInt* handler = handlers;

	while(handler) {
		uint8_t num = (handler->port == 1) ? 24 + handler->pin : handler->pin;

		if(LPC_SYSCON->PINTSEL[idx] == num) {

			_currentPort = handler->port;
			_currentPin = handler->pin;
			_currentEdge = 0;

			if(LPC_GPIO_PIN_INT->FALL & (1<<idx)) {
				_currentEdge = IntEdge::FALLING_EDGE;
			}
			if(LPC_GPIO_PIN_INT->RISE & (1<<idx)) {
				_currentEdge = IntEdge::RISING_EDGE;
			}

			LPC_GPIO_PIN_INT->IST |= (1<<idx); //clear int pending

			if(handler->func)
				handler->func();

			break;
		}
		handler = handler->next;
	}
}

extern "C"
void FLEX_INT0_IRQHandler() {
	IRQWrapper w;
	GpioInt::_handleFLEXInt(0);
}
extern "C"
void FLEX_INT1_IRQHandler() {
	IRQWrapper w;
	GpioInt::_handleFLEXInt(1);
}
extern "C"
void FLEX_INT2_IRQHandler() {
	IRQWrapper w;
	GpioInt::_handleFLEXInt(2);
}
extern "C"
void FLEX_INT3_IRQHandler() {
	IRQWrapper w;
	GpioInt::_handleFLEXInt(3);
}
extern "C"
void FLEX_INT4_IRQHandler() {
	IRQWrapper w;
	GpioInt::_handleFLEXInt(4);
}
extern "C"
void FLEX_INT5_IRQHandler() {
	IRQWrapper w;
	GpioInt::_handleFLEXInt(5);
}
extern "C"
void FLEX_INT6_IRQHandler() {
	IRQWrapper w;
	GpioInt::_handleFLEXInt(6);
}
extern "C"
void FLEX_INT7_IRQHandler() {
	IRQWrapper w;
	GpioInt::_handleFLEXInt(7);
}

void lpc11u::GpioInterrupt::enableInterrupt(uint32_t port, uint32_t pin, IntEdge edge) {

	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<19); //enable clock to GPIOINT block

	uint8_t pinIdx = 24*port + pin;
	uint8_t irqNum = 0;

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
	uint32_t irqmsk = 1<<irqNum;

	if(edge & (IntEdge::LEVEL_HIGH|IntEdge::LEVEL_LOW)) {
		LPC_GPIO_PIN_INT->ISEL |= irqmsk; //level sensitive

		if(edge & (IntEdge::LEVEL_HIGH)) {
			LPC_GPIO_PIN_INT->IENF |= irqmsk;
			LPC_GPIO_PIN_INT->IENR |= irqmsk;
			LPC_GPIO_PIN_INT->SIENF |= irqmsk;
		} else {
			LPC_GPIO_PIN_INT->IENF &= ~(irqmsk);
			LPC_GPIO_PIN_INT->IENR |= irqmsk;
			LPC_GPIO_PIN_INT->CIENF |= irqmsk;
		}

	} else {
		LPC_GPIO_PIN_INT->ISEL &= ~(irqmsk); //edge sensitive

		if((edge & IntEdge::RISING_EDGE) && (edge & IntEdge::FALLING_EDGE)) {
			//XPCC_LOG_DEBUG .printf("double edge\n");
			LPC_GPIO_PIN_INT->IENR |= irqmsk;
			LPC_GPIO_PIN_INT->IENF |= irqmsk;
		} else {
			if(edge & IntEdge::RISING_EDGE) {
				LPC_GPIO_PIN_INT->IENR |= irqmsk;
				//XPCC_LOG_DEBUG .printf("rising enable %x\n", LPC_GPIO_PIN_INT->IENR);
			} else {
				//XPCC_LOG_DEBUG .printf("falling enable\n");
				LPC_GPIO_PIN_INT->IENF |= irqmsk;
			}
		}
	}
	LPC_GPIO_PIN_INT->IST = irqmsk;
	NVIC_EnableIRQ((IRQn_Type)(FLEX_INT0_IRQn + irqNum));

}

void lpc11u::GpioInterrupt::disableInterrupt(uint8_t port, uint8_t pin) {

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
}
