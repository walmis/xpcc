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
void (*Usart3::txCallback)() = 0;

template<>
void (*Usart3::rxCallback)() = 0;

template<>
void Usart3::enable() {
	// enable clock
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	// reset timer
	RCC->APB1RSTR |= RCC_APB1RSTR_USART3RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_USART3RST;

	USART1->CR1 |= USART_CR1_UE;// Uart Enable
}
template<>
void Usart3::enableInterruptVector(bool enable, uint32_t priority)
{
	if (enable) {
		// Set priority for the interrupt vector
		NVIC_SetPriority(USART3_IRQn, priority);

		// register IRQ at the NVIC
		NVIC_EnableIRQ (USART3_IRQn);
	} else {
		NVIC_DisableIRQ (USART3_IRQn);
	}
}

template<>
void Usart3::disable() {
	RCC->APB1ENR &= ~RCC_APB1ENR_USART3EN;
}

extern "C" void
USART3_IRQHandler()
{
	Usart3::handleIRQ();
} // IRQHandler

}
}
