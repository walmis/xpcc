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
void (*Usart1::txCallback)() = 0;

template<>
void (*Usart1::rxCallback)() = 0;

template<>
void Usart1::enable() {
	// enable clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	// reset timer
	RCC->APB2RSTR |= RCC_APB2RSTR_USART1RST;
	RCC->APB2RSTR &= ~RCC_APB2RSTR_USART1RST;

	USART1->CR1 |= USART_CR1_UE;// Uart Enable
}

template<>
void Usart1::disable() {
	RCC->APB2ENR &= ~RCC_APB2ENR_USART1EN;
}

extern "C" void
USART1_IRQHandler()
{
	Usart1::handleIRQ();
} // IRQHandler

}
}
