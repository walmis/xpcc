/*
 * gpio_interrupt.cpp
 *
 *  Created on: Feb 1, 2016
 *      Author: walmis
 */

#include "gpio_interrupt.h"

namespace xpcc {


static volatile uint8_t _currentEdge;
static volatile uint8_t _currentPort;
static volatile uint8_t _currentPin;

GpioInt::Entry* interrupt_list;


static void handle_int(uint32_t irqn) {
	GpioInt::Entry* e = interrupt_list;
	while(e) {
		uint32_t pinmsk = (1<< e->pin);

		if(e->port == 0 && irqn == EINT0_IRQn) {
			if(LPC_GPIO0->MIS & pinmsk) {
				LPC_GPIO0->IC |= pinmsk;
				if(e->func) e->func();
				break;
			}
		} else if(e->port == 1 && irqn ==  EINT1_IRQn) {
			if(LPC_GPIO1->MIS & pinmsk) {
				LPC_GPIO1->IC |= pinmsk;
				if(e->func) e->func();
				break;
			}
		} else if(e->port == 2 && irqn == EINT2_IRQn) {
			if(LPC_GPIO2->MIS & pinmsk) {
				LPC_GPIO2->IC |= pinmsk;
				if(e->func) e->func();
				break;
			}
		} else if(e->port == 3 && irqn == EINT3_IRQn) {
			if(LPC_GPIO3->MIS & pinmsk) {
				LPC_GPIO3->IC |= pinmsk;
				if(e->func) e->func();
				break;
			}
		}

		e = e->next;
	}
}

extern "C"
void PIOINT0_IRQHandler() {
	handle_int(EINT0_IRQn);
}

extern "C"
void PIOINT1_IRQHandler() {
	handle_int(EINT1_IRQn);
}

extern "C"
void PIOINT2_IRQHandler() {
	handle_int(EINT2_IRQn);
}

extern "C"
void PIOINT3_IRQHandler() {
	handle_int(EINT3_IRQn);
}


GpioInt::GpioInt(uint8_t port, uint8_t pin, IntEdge edges, xpcc::function<void()> callback) {
			int_info.port = port;
			int_info.pin = pin;
			int_info.edges = (uint8_t)edges;
			int_info.next = 0;
			int_info.func = callback;

			if(interrupt_list == 0) {
				interrupt_list = &int_info;
			} else {
				xpcc::atomic::Lock l;
				GpioInt::Entry* e = interrupt_list;
				while(e->next) {
					e = e->next;
				}
				e->next = &int_info;
			}

			enableInterrupts();
}

void GpioInt::enable() {
	enableInterrupt(int_info.port, int_info.pin, (IntEdge)int_info.edges);
}

void GpioInt::disable() {
	disableInterrupt(int_info.port, int_info.pin);
}

}
