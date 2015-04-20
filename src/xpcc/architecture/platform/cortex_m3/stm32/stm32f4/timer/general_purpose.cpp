// coding: utf-8
/* Copyright (c) 2013, Roboterclub Aachen e.V.
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#include "general_purpose.hpp"

namespace xpcc {
namespace stm32 {

template<>
void GPTimer2::enable() {
	RCC->APB1ENR  |=  RCC_APB1ENR_TIM2EN;
	// reset timer
	RCC->APB1RSTR |=  RCC_APB1RSTR_TIM2RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM2RST;
}

template<>
void GPTimer3::enable() {
	RCC->APB1ENR  |=  RCC_APB1ENR_TIM3EN;
	// reset timer
	RCC->APB1RSTR |=  RCC_APB1RSTR_TIM3RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM3RST;
}

template<>
void GPTimer4::enable() {
	RCC->APB1ENR  |=  RCC_APB1ENR_TIM4EN;
	// reset timer
	RCC->APB1RSTR |=  RCC_APB1RSTR_TIM4RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM4RST;
}

template<>
void GPTimer5::enable() {
	RCC->APB1ENR  |=  RCC_APB1ENR_TIM5EN;
	// reset timer
	RCC->APB1RSTR |=  RCC_APB1RSTR_TIM5RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM5RST;
}
template<>
void GPTimer9::enable() {
	RCC->APB2ENR  |=  RCC_APB2ENR_TIM9EN;
	// reset timer
	RCC->APB2RSTR |=  RCC_APB2RSTR_TIM9RST;
	RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM9RST;
}

template<>
void GPTimer10::enable() {
	RCC->APB2ENR  |=  RCC_APB2ENR_TIM10EN;
	// reset timer
	RCC->APB2RSTR |=  RCC_APB2RSTR_TIM10RST;
	RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM11RST;
}
template<>
void GPTimer11::enable() {
	RCC->APB2ENR  |=  RCC_APB2ENR_TIM11EN;
	// reset timer
	RCC->APB2RSTR |=  RCC_APB2RSTR_TIM11RST;
	RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM11RST;
}
template<>
void GPTimer12::enable() {
	RCC->APB1ENR  |=  RCC_APB1ENR_TIM12EN;
	// reset timer
	RCC->APB1RSTR |=  RCC_APB1RSTR_TIM12RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM12RST;
}

template<>
void GPTimer13::enable() {
	RCC->APB1ENR  |=  RCC_APB1ENR_TIM13EN;
	// reset timer
	RCC->APB1RSTR |=  RCC_APB1RSTR_TIM13RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM13RST;
}

template<>
void GPTimer14::enable() {
	RCC->APB1ENR  |=  RCC_APB1ENR_TIM14EN;
	// reset timer
	RCC->APB1RSTR |=  RCC_APB1RSTR_TIM14RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM14RST;
}

template<>
void GPTimer2::disable() {
	RCC->APB1ENR &= ~RCC_APB1ENR_TIM2EN;

	TIM2->CR1 = 0;
	TIM2->DIER = 0;
	TIM2->CCER = 0;
}
template<>
void GPTimer3::disable() {
	RCC->APB1ENR &= ~RCC_APB1ENR_TIM3EN;

	TIM3->CR1 = 0;
	TIM3->DIER = 0;
	TIM3->CCER = 0;
}
template<>
void GPTimer4::disable() {
	RCC->APB1ENR &= ~RCC_APB1ENR_TIM4EN;

	TIM4->CR1 = 0;
	TIM4->DIER = 0;
	TIM4->CCER = 0;
}
template<>
void GPTimer5::disable() {
	RCC->APB1ENR &= ~RCC_APB1ENR_TIM5EN;

	TIM5->CR1 = 0;
	TIM5->DIER = 0;
	TIM5->CCER = 0;
}
template<>
void GPTimer9::disable() {
	RCC->APB2ENR &= ~RCC_APB2ENR_TIM9EN;

	TIM9->CR1 = 0;
	TIM9->DIER = 0;
	TIM9->CCER = 0;
}
template<>
void GPTimer10::disable() {
	RCC->APB2ENR &= ~RCC_APB2ENR_TIM10EN;

	TIM10->CR1 = 0;
	TIM10->DIER = 0;
	TIM10->CCER = 0;
}
template<>
void GPTimer11::disable() {
	RCC->APB2ENR &= ~RCC_APB2ENR_TIM11EN;

	TIM11->CR1 = 0;
	TIM11->DIER = 0;
	TIM11->CCER = 0;
}
template<>
void GPTimer12::disable() {
	RCC->APB1ENR &= ~RCC_APB1ENR_TIM12EN;

	TIM12->CR1 = 0;
	TIM12->DIER = 0;
	TIM12->CCER = 0;
}
template<>
void GPTimer13::disable() {
	RCC->APB1ENR &= ~RCC_APB1ENR_TIM13EN;

	TIM13->CR1 = 0;
	TIM13->DIER = 0;
	TIM13->CCER = 0;
}

template<>
void GPTimer14::disable() {
	RCC->APB1ENR &= ~RCC_APB1ENR_TIM14EN;

	TIM14->CR1 = 0;
	TIM14->DIER = 0;
	TIM14->CCER = 0;
}


}
}



// ----------------------------------------------------------------------------
