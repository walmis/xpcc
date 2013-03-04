/*
 * gpio_interrupt.cpp
 *
 *  Created on: Feb 20, 2013
 *      Author: walmis
 */

#include "gpio_interrupt.h"

namespace xpcc {
namespace lpc17 {

static GpioIntHandler handlers[2];

extern "C" void EINT3_IRQHandler()
{
	if((LPC_GPIOINT->IntStatus & 1)) {
		for(int i = 0; i < 31; i++) {
			if(i == 12) i = 15; //skip 12:14 according to datasheet
			int mask = (1<<i);

			if(LPC_GPIOINT->IO0IntStatR & mask) {
				if(handlers[0])
					handlers[0](i, IntEvent::RISING_EDGE);
			}
			if(LPC_GPIOINT->IO0IntStatF & mask) {
				if(handlers[0])
					handlers[0](i, IntEvent::FALLING_EDGE);
			}

			LPC_GPIOINT->IO0IntClr = mask;
		}

	} else if((LPC_GPIOINT->IntStatus & 4)) {

		for(int i = 0; i < 14; i++) {
			int mask = (1<<i);

			if(LPC_GPIOINT->IO2IntStatR & mask) {
				if(handlers[1])
					handlers[1](i, IntEvent::RISING_EDGE);
			}
			if(LPC_GPIOINT->IO2IntStatF & mask) {
				if(handlers[1])
					handlers[1](i, IntEvent::FALLING_EDGE);
			}

			LPC_GPIOINT->IO2IntClr = mask;
		}
	}
}

} /* namespace lpc */
} /* namespace xpcc */

void xpcc::lpc17::GpioInterrupt::registerPortHandler(uint8_t port,
		GpioIntHandler func) {

	if(port == 0) {
		handlers[0] = func;
	} else if(port == 2) {
		handlers[1] = func;
	}

}
