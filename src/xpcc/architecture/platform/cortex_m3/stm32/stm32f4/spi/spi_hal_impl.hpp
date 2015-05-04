// coding: utf-8
/* Copyright (c) 2013, Roboterclub Aachen e.V.
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef XPCC_STM32_SPI_HAL_HPP
#	error 	"Don't include this file directly, use 'spi_hal.hpp' instead!"
#endif

static SPI_TypeDef* const spi_tab[] = {SPI1, SPI2, SPI3};
#define SPIx spi_tab[spiid-1]

template <int spiid>
void inline
xpcc::stm32::SpiHal<spiid>::enable()
{
	switch(spiid) {
	case 1:
		// enable clock
		RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
		// reset spi
		RCC->APB2RSTR |=  RCC_APB2RSTR_SPI1RST;
		RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;
		break;
	case 2:
		// enable clock
		RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
		// reset spi
		RCC->APB1RSTR |=  RCC_APB1RSTR_SPI2RST;
		RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI2RST;
		break;
	case 3:
		// enable clock
		RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
		// reset spi
		RCC->APB1RSTR |=  RCC_APB1RSTR_SPI3RST;
		RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI3RST;
	}
	SPIx->CR1 |= SPI_CR1_SPE;		// SPI Enable
}

template <int spiid>
void inline
xpcc::stm32::SpiHal<spiid>::disable()
{
	// disable clock
	switch(spiid) {
	case 1:
		RCC->APB2ENR &= ~RCC_APB2ENR_SPI1EN;
		break;
	case 2:
		RCC->APB1ENR &= ~RCC_APB1ENR_SPI2EN;
		break;
	case 3:
		RCC->APB1ENR &= ~RCC_APB1ENR_SPI3EN;
		break;
	}
}

template <int spiid>
void inline
xpcc::stm32::SpiHal<spiid>::initialize(Prescaler prescaler,
		MasterSelection masterSelection, DataMode dataMode,
		DataOrder dataOrder, DataSize dataSize)
{
	enable();
	// disable peripheral
	SPIx->CR1 &= ~SPI_CR1_SPE;
	// set parameters
	SPIx->CR1 = 	  static_cast<uint32_t>(dataMode)
						| static_cast<uint32_t>(dataOrder)
						| static_cast<uint32_t>(dataSize)
						| static_cast<uint32_t>(masterSelection)
#if defined(STM32F1XX) || defined(STM32F2XX) || defined(STM32F4XX)
						| static_cast<uint32_t>(prescaler);
#elif defined(STM32F0XX) || defined(STM32F3XX)
	SPIx->CR2 = static_cast<uint32_t>(prescaler);
#endif
	if(masterSelection == MasterSelection::Master) {
		SPIx->CR2 |=  SPI_CR2_SSOE; // for master mode
	}
	// reenable peripheral
	SPIx->CR1 |= SPI_CR1_SPE;
}

template <int spiid>
void inline
xpcc::stm32::SpiHal<spiid>::setDataMode(DataMode dataMode)
{
	SPIx->CR1 = (SPIx->CR1 & ~static_cast<uint32_t>(DataMode::All))
										 | static_cast<uint32_t>(dataMode);
}

template <int spiid>
void inline
xpcc::stm32::SpiHal<spiid>::setDataOrder(DataOrder dataOrder)
{
	SPIx->CR1 = (SPIx->CR1 & ~static_cast<uint32_t>(DataOrder::All))
										 | static_cast<uint32_t>(dataOrder);
}

template <int spiid>
void inline
xpcc::stm32::SpiHal<spiid>::setDataSize(DataSize dataSize)
{
#if defined(STM32F0XX) || defined(STM32F3XX)
	SPIx->CR2 = (SPIx->CR2 & ~static_cast<uint32_t>(DataSize::All))
										 | static_cast<uint32_t>(dataSize);
#endif
#if defined(STM32F1XX) || defined(STM32F2XX) || defined(STM32F4XX)
	// TODO: implement as set/reset bit
	SPIx->CR1 = (SPIx->CR1 & ~static_cast<uint32_t>(DataSize::All))
										 | static_cast<uint32_t>(dataSize);
#endif
}

template <int spiid>
void inline
xpcc::stm32::SpiHal<spiid>::setMasterSelection(MasterSelection masterSelection)
{
	// TODO: implement as set/reset bit
	SPIx->CR1 = (SPIx->CR1 & ~static_cast<uint32_t>(MasterSelection::All))
										 | static_cast<uint32_t>(masterSelection);
}

template <int spiid>
inline bool
xpcc::stm32::SpiHal<spiid>::isReceiveRegisterNotEmpty()
{
	return getInterruptFlags() & InterruptFlag::RxBufferNotEmpty;
}

template <int spiid>

inline bool
xpcc::stm32::SpiHal<spiid>::isTransmitRegisterEmpty()
{
	return getInterruptFlags() & InterruptFlag::TxBufferEmpty;
}

template <int spiid>
void inline
xpcc::stm32::SpiHal<spiid>::write(uint16_t data)
{
	SPIx->DR = data;
}

template <int spiid>
void inline
xpcc::stm32::SpiHal<spiid>::read(uint8_t &data)
{
	data = static_cast<uint8_t>(SPIx->DR);
}

template <int spiid>
void inline
xpcc::stm32::SpiHal<spiid>::read(uint16_t &data)
{
	data = static_cast<uint16_t>(SPIx->DR);
}

template <int spiid>
void inline
xpcc::stm32::SpiHal<spiid>::enableInterruptVector(bool enable, uint32_t priority)
{
	uint8_t irqn = 0;
	switch(spiid) {
	case 1:
		irqn = SPI1_IRQn;
		break;
	case 2:
		irqn = SPI2_IRQn;
		break;
	case 3:
		irqn = SPI3_IRQn;
		break;
	}
	if (enable) {
		// Set priority for the interrupt vector

		NVIC_SetPriority(irqn, priority);
		// register IRQ at the NVIC
		NVIC_EnableIRQ(irqn);
	}
	else {
		NVIC_DisableIRQ(irqn);
	}
}

template <int spiid>
void inline
xpcc::stm32::SpiHal<spiid>::enableInterrupt(Interrupt interrupt)
{
	SPIx->CR2 |= static_cast<uint32_t>(interrupt);
}

template <int spiid>
void inline
xpcc::stm32::SpiHal<spiid>::disableInterrupt(Interrupt interrupt)
{
	SPIx->CR2 &= ~static_cast<uint32_t>(interrupt);
}

template <int spiid>
xpcc::stm32::SpiBase::InterruptFlag inline
xpcc::stm32::SpiHal<spiid>::getInterruptFlags()
{
	return static_cast<InterruptFlag>(SPIx->SR );
}

template <int spiid>
void inline
xpcc::stm32::SpiHal<spiid>::acknowledgeInterruptFlag(InterruptFlag /*flags*/)
{
	// TODO: implement; see STM32F3 reference manual p. 736
	// SPIx->SR =  static_cast<InterruptFlag>(flags);
}
