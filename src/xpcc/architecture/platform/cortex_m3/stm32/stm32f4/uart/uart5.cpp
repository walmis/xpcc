/*
 * usart1.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: walmis
 */

#include "uart_hal.hpp"

namespace xpcc {
namespace stm32 {

template<>
void (*Uart5::txCallback)() = 0;

template<>
void (*Uart5::rxCallback)() = 0;

template<>
void Uart5::enable() {
	// enable clock
	RCC->APB1ENR |= RCC_APB1ENR_UART5EN;
	// reset timer
	RCC->APB1RSTR |= RCC_APB1RSTR_UART5RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_UART5RST;

	USART1->CR1 |= USART_CR1_UE;// Uart Enable
}
template<>
void Uart5::enableInterruptVector(bool enable, uint32_t priority)
{
	if (enable) {
		// Set priority for the interrupt vector
		NVIC_SetPriority(UART5_IRQn, priority);

		// register IRQ at the NVIC
		NVIC_EnableIRQ (UART5_IRQn);
	} else {
		NVIC_DisableIRQ (UART5_IRQn);
	}
}

template<>
void Uart5::disable() {
	RCC->APB1ENR &= ~RCC_APB1ENR_UART5EN;
}

extern "C" void
UART5_IRQHandler()
{
	Uart5::handleIRQ();
} // IRQHandler

}
}
