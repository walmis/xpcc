/*
 * adc.cpp
 *
 *  Created on: May 20, 2015
 *      Author: walmis
 */

#include "adc.hpp"

namespace xpcc {
namespace stm32 {

template<>
void xpcc::stm32::Adc1::initialize(Prescaler pre) {
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;	// start ADC Clock
	ADC1->CR2 |= ADC_CR2_ADON;			// switch on ADC
	ADC1->SQR1 = 0;
	ADC1->SQR2 = 0;
	ADC1->SQR3 = 0;
	setPrescaler(pre);
}

template<>
void xpcc::stm32::Adc2::initialize(Prescaler pre) {
	RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;	// start ADC Clock
	ADC2->CR2 |= ADC_CR2_ADON;			// switch on ADC
	setPrescaler(pre);
}

template<>
void xpcc::stm32::Adc3::initialize(Prescaler pre) {
	RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;	// start ADC Clock
	ADC3->CR2 |= ADC_CR2_ADON;			// switch on ADC
	setPrescaler(pre);
}

template<>
void xpcc::stm32::Adc1::shutdownAdc() {
	ADC1->CR2 &= ~(ADC_CR2_ADON);		// switch off ADC
	RCC->APB2ENR &= ~RCC_APB2ENR_ADC1EN; // stop ADC Clock
}

template<>
void xpcc::stm32::Adc2::shutdownAdc() {
	ADC2->CR2 &= ~(ADC_CR2_ADON);		// switch off ADC
	RCC->APB2ENR &= ~RCC_APB2ENR_ADC2EN; // stop ADC Clock
}

template<>
void xpcc::stm32::Adc3::shutdownAdc() {
	ADC3->CR2 &= ~(ADC_CR2_ADON);		// switch off ADC
	RCC->APB2ENR &= ~RCC_APB2ENR_ADC3EN; // stop ADC Clock
}

}
}
