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
void (*Usart6::txCallback)() = 0;

template<>
void (*Usart6::rxCallback)() = 0;

template<>
void Usart6::enable() {
	// enable clock
	RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
	// reset timer
	RCC->APB2RSTR |= RCC_APB2RSTR_USART6RST;
	RCC->APB2RSTR &= ~RCC_APB2RSTR_USART6RST;

	USART6->CR1 |= USART_CR1_UE;// Uart Enable
}
template<>
void Usart6::enableInterruptVector(bool enable, uint32_t priority)
{
	if (enable) {
		// Set priority for the interrupt vector
		NVIC_SetPriority(USART6_IRQn, priority);

		// register IRQ at the NVIC
		NVIC_EnableIRQ (USART6_IRQn);
	} else {
		NVIC_DisableIRQ (USART6_IRQn);
	}
}

template<>
void Usart6::disable() {
	RCC->APB2ENR &= ~RCC_APB2ENR_USART6EN;
}

extern "C" void
USART6_IRQHandler()
{
	Usart6::handleIRQ();
} // IRQHandler

}
}
