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
void (*Usart2::txCallback)() = 0;

template<>
void (*Usart2::rxCallback)() = 0;

template<>
void Usart2::enable() {
	// enable clock
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	// reset timer
	RCC->APB1RSTR |= RCC_APB1RSTR_USART2RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_USART2RST;

	USART1->CR1 |= USART_CR1_UE;// Uart Enable
}
template<>
void Usart2::enableInterruptVector(bool enable, uint32_t priority)
{
	if (enable) {
		// Set priority for the interrupt vector
		NVIC_SetPriority(USART2_IRQn, priority);

		// register IRQ at the NVIC
		NVIC_EnableIRQ (USART2_IRQn);
	} else {
		NVIC_DisableIRQ (USART2_IRQn);
	}
}

template<>
void Usart2::disable() {
	RCC->APB1ENR &= ~RCC_APB1ENR_USART2EN;
}

extern "C" void
USART2_IRQHandler()
{
	Usart2::handleIRQ();
} // IRQHandler

}
}
