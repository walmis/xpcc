/*
 * gpio_interrupt.cpp
 *
 *  Created on: Feb 20, 2013
 *      Author: walmis
 */

#include "gpio_interrupt.h"

namespace xpcc {
namespace lpc {

static GpioIntHandler handlers[4];

static void intHandler(LPC_GPIO_TypeDef* port, uint8_t portNum) {
	for(uint8_t i = 0; i < 32; i++) {
		if((port->MIS >> i) & 0x01) {
			if(handlers[portNum]) {
				handlers[portNum](i);
				port->IC |= (0x1<<i);
			}
		}
	}
}

extern "C" void PIOINT0_IRQHandler(void)
{
	intHandler(LPC_GPIO0, 0);
}
extern "C" void PIOINT1_IRQHandler(void)
{
	intHandler(LPC_GPIO1, 1);
}
extern "C" void PIOINT2_IRQHandler(void)
{
	intHandler(LPC_GPIO2, 2);
}
extern "C" void PIOINT3_IRQHandler(void)
{
	intHandler(LPC_GPIO3, 3);
}



} /* namespace lpc */
} /* namespace xpcc */

void xpcc::lpc::GpioInterrupt::registerPortHandler(uint8_t port,
		GpioIntHandler func) {

	if(port >= 0 && port < 4) {
		handlers[port] = func;
	}

}
