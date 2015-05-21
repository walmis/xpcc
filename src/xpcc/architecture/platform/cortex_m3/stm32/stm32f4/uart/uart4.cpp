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
void (*Uart4::txCallback)() = 0;

template<>
void (*Uart4::rxCallback)() = 0;

template<>
void Uart4::enable() {
	// enable clock
	RCC->APB1ENR |= RCC_APB1ENR_UART4EN;
	// reset timer
	RCC->APB1RSTR |= RCC_APB1RSTR_UART4RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_UART4RST;

	USART1->CR1 |= USART_CR1_UE;// Uart Enable
}
template<>
void Uart4::enableInterruptVector(bool enable, uint32_t priority)
{
	if (enable) {
		// Set priority for the interrupt vector
		NVIC_SetPriority(UART4_IRQn, priority);

		// register IRQ at the NVIC
		NVIC_EnableIRQ (UART4_IRQn);
	} else {
		NVIC_DisableIRQ (UART4_IRQn);
	}
}

template<>
void Uart4::disable() {
	RCC->APB1ENR &= ~RCC_APB1ENR_UART4EN;
}

extern "C" void
UART4_IRQHandler()
{
	Uart4::handleIRQ();
} // IRQHandler

}
}
