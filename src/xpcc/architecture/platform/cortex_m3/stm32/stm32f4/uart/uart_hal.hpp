// coding: utf-8
/* Copyright (c) 2013, Roboterclub Aachen e.V.
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------
//%% if id in [1, 2, 3, 6]
//%%	set uart = "Usart"
//%% elif id in [4, 5]
//%%	set uart = "Uart"
//%% endif
#pragma once

#include <stdint.h>
#include "uart_base.hpp"
#include "../rcc.hpp"

namespace xpcc {
namespace stm32 {
/**
 * Universal asynchronous receiver transmitter ({{ uart ~ id }})
 *
 * Not available on the low- and medium density devices.
 *
 * Very basic implementation that exposes more hardware features than
 * the regular Usart classes.
 *
 * @author		Kevin Laeufer
 * @ingroup		stm32
 */

USART_TypeDef* const _uart[] = {USART1, USART2, USART3, UART4, UART5, USART6};

#define UARTx (_uart[uartid-1])

template<int uartid>
class Uart: public UartBase {

public:

	/**
	 * Initialize Uart HAL Peripheral
	 *
	 * Enables clocks, the UART peripheral (but neither TX nor RX)
	 * Sets baudrate and parity.
	 */
//	template<class clockSource, uint32_t baudrate, OversamplingMode oversample =
//			OversamplingMode::By16>
//	static inline void
//	initialize(Parity parity = Parity::Disabled);
//
//	/// Choose if you want to oversample by 16 (_default_) or by 8
//	static inline void
//	setOversamplingMode(OversamplingMode mode);
////%% if id in [1, 2, 3, 6]
//	// Methods needed to use this Usart Peripheral for SPI
//	static inline void
//	setSpiClock(SpiClock clk);
//
//	static inline void
//	setSpiDataMode(SpiDataMode mode);
//
//	static inline void
//	setLastBitClockPulse(LastBitClockPulse pulse);
////%% endif
//	/**
//	 * \brief	Write a single byte to the transmit register
//	 *
//	 * @warning 	This method does NOT do any sanity checks!!
//	 *				It is your responsibility to check if the register
//	 *				is empty!
//	 */
//	static inline void
//	write(uint8_t data);
//
//	/**
//	 * Saves the value of the receive register to data
//	 *
//	 * @warning 	This method does NOT do any sanity checks!!
//	 *				It is your responsibility to check if the register
//	 *				contains something useful!
//	 */
//	static inline void
//	read(uint8_t &data);
//
//	/// Enable/Disable Transmitter
//	static inline void
//	setTransmitterEnable(const bool enable);
//
//	/// Enable/Disable Receiver
//	static inline void
//	setReceiverEnable(bool enable);
//
//	/// Returns true if data has been received
//	static inline bool
//	isReceiveRegisterNotEmpty();
//
//	/// Returns true if data can be written
//	static inline bool
//	isTransmitRegisterEmpty();
//
//	static inline void
//	enableInterruptVector(bool enable, uint32_t priority);
//
//	static inline void
//	enableInterrupt(Interrupt interrupt);
//
//	static inline void
//	disableInterrupt(Interrupt interrupt);
//
//	static inline InterruptFlag
//	getInterruptFlags();
//
//	/**
//	 * Returns the value of the receive register
//	 *
//	 * @warning 	Not all InterruptFlags can be cleared this way.
//	 */
//	static inline void
//	resetInterruptFlags(InterruptFlag flags);

	static void setParity(const Parity parity) {
		uint32_t flags = UARTx->CR1;
		flags &= ~(USART_CR1_PCE | USART_CR1_PS | USART_CR1_M);
		flags |= static_cast<uint32_t>(parity);
		if (parity != Parity::Disabled) {
			// Parity Bit counts as 9th bit -> enable 9 data bits
			flags |= USART_CR1_M;
		}
		UARTx->CR1 = flags;
	}

	static uint8_t getFifoSize() {
		return 1;
	}

	static uint16_t getBaudrateSettings(uint32_t baudrate,
			OversamplingMode oversample = OversamplingMode::By8) {
		if (oversample == OversamplingMode::By16) {

			// see http://www.mikrocontroller.net/topic/143715
			return ((2 * getClock()) / baudrate + 1) / 2;
		} else {
			// see http://www.mikrocontroller.net/topic/143715
			uint16_t brr = ((2 * getClock()) / (baudrate / 2) + 1) / 2;
			// correct baudrate
//	%% if target is stm32f1 or target is stm32f2 or target is stm32f4
			return (USART_BRR_DIV_Mantissa & brr)
					| ((USART_BRR_DIV_Fraction & brr) >> 1);
//	%% elif target is stm32f3 or target is stm32f0
//			return (USART_BRR_DIV_MANTISSA & brr) | ((USART_BRR_DIV_FRACTION & brr)>>1);
//	%% endif
		}
	}

	static void enable();
//	{
//
//		if (uartid == 1 || uartid == 6) {
//			// enable clock
//			RCC->APB2ENR |= RCC_APB2ENR_UARTxEN;
//			// reset timer
//			RCC->APB2RSTR |= RCC_APB2RSTR_UARTxRST;
//			RCC->APB2RSTR &= ~RCC_APB2RSTR_UARTxRST;
//		} else {
//
//			// FIXME: there seems to be a bug in the stm32f3xxlib which does not provide
//			//        the necessary RCC_APB1ENR_UART5EN define and probably defines
//			//        RCC_APB1ENR_UART4EN incorrectly (0x00100000 instead of 0x00080000)
//			// enable clock
//			RCC->APB1ENR |= RCC_APB1ENR_UARTxEN;
//			// reset uart
//			RCC->APB1RSTR |= RCC_APB1RSTR_UARTxRST;
//			RCC->APB1RSTR &= ~RCC_APB1RSTR_UARTxRST;
//		}
//		UARTx->CR1 |= USART_CR1_UE;		// Uart Enable
//	}

	static void disable();
//	{
//		// TX, RX, Uart, etc. Disable
//		UARTx->CR1 = 0;
////	%% if id in [1, 6]
//		// disable clock
////		RCC->APB2ENR &= ~RCC_APB2ENR_UARTxEN;
////	%% elif id in [2, 3, 4, 5]
//		// FIXME: there seems to be a bug in the stm32f3xxlib which does not provide
//		//        the necessary RCC_APB1ENR_UART5EN define and probably defines
//		//        RCC_APB1ENR_UART4EN incorrectly (0x00100000 instead of 0x00080000)
//		// disable clock
//		RCC->APB1ENR &= ~RCC_APB1ENR_UARTxEN;
////	%% endif
//	}

	static void initialize(uint32_t baudrate, Parity parity = Parity::Disabled, OversamplingMode oversample =OversamplingMode::By8) {
		enable();
		// DIRTY HACK: disable and reenable uart to be able to set
		//             baud rate as well as parity
		UARTx->CR1 &= ~USART_CR1_UE;	// Uart Disable
		// set baudrate
		UARTx->BRR = getBaudrateSettings(baudrate, oversample);
		setParity(parity);
		setOversamplingMode(oversample);
		UARTx->CR1 |= USART_CR1_UE;	// Uart Reenable

		setTransmitterEnable(true);
		setReceiverEnable(true);
	}

	static void setOversamplingMode(OversamplingMode mode) {
		if (mode == OversamplingMode::By16) {
			UARTx->CR1 &= ~static_cast<uint32_t>(OversamplingMode::By8);
		} else {
			UARTx->CR1 |= static_cast<uint32_t>(OversamplingMode::By8);
		}
	}
//	%% if id in [1, 2, 3, 6]

	static void setSpiClock(SpiClock clk) {
		if (clk == SpiClock::Disabled) {
			UARTx->CR2 &= ~static_cast<uint32_t>(SpiClock::Enabled);
		} else {
			UARTx->CR2 |= static_cast<uint32_t>(SpiClock::Enabled);
		}
	}

	static void setSpiDataMode(SpiDataMode mode) {
		UARTx->CR2 = (UARTx->CR2 & ~static_cast<uint32_t>(SpiDataMode::Mode3))
				| static_cast<uint32_t>(mode);
	}

	static void setLastBitClockPulse(LastBitClockPulse pulse) {
		if (pulse == LastBitClockPulse::DoNotOutput) {
			UARTx->CR2 &= ~static_cast<uint32_t>(LastBitClockPulse::Output);
		} else {
			UARTx->CR2 |= static_cast<uint32_t>(LastBitClockPulse::Output);
		}
	}
//	%% endif

	static void put(uint8_t data) {
		UARTx->DR = data;
	}

	static uint8_t get() {
		return UARTx->DR;
	}

	static void setTransmitterEnable(const bool enable) {
		if (enable) {
			UARTx->CR1 |= USART_CR1_TE;
		} else {
			UARTx->CR1 &= ~USART_CR1_TE;
		}
	}

	static void setReceiverEnable(bool enable) {
		if (enable) {
			UARTx->CR1 |= USART_CR1_RE;
		} else {
			UARTx->CR1 &= ~USART_CR1_RE;
		}
	}

	static bool isReceiveRegisterNotEmpty() {
//	%% if target is stm32f0 or target is stm32f3
//		return UARTx->ISR & USART_ISR_RXNE;
//	%% elif target is stm32f2 or target is stm32f4
		return UARTx->SR & USART_SR_RXNE;
//	%% endif
	}

	static bool isTransmitRegisterEmpty() {
//	%% if target is stm32f0 or target is stm32f3
//		return UARTx->ISR & USART_ISR_TXE;
//	%% elif target is stm32f2 or target is stm32f4
		return UARTx->SR & USART_SR_TXE;
//	%% endif
	}

	static void enableInterruptVector(bool enable, uint32_t priority);
//	{
//		if (enable) {
//			// Set priority for the interrupt vector
//			NVIC_SetPriority(UARTx_IRQn, priority);
//
//			// register IRQ at the NVIC
//			NVIC_EnableIRQ (UARTx_IRQn);
//		} else {
//			NVIC_DisableIRQ (UARTx_IRQn);
//		}
//	}

	static void enableTxCompleteInterrupt(bool en) {
		if(en)
			enableInterrupt(Interrupt::TxComplete);
		else
			disableInterrupt(Interrupt::TxComplete);
	}

	static void enableRxCompleteInterrupt(bool en) {
		if(en)
			enableInterrupt(Interrupt::RxNotEmpty);
		else
			disableInterrupt(Interrupt::RxNotEmpty);
	}


	static void attachTxCompleteInterrupt(void (*f)()) {
		txCallback = f;
	}

	static void attachRxCompleteInterrupt(void (*f)()) {
		rxCallback = f;
	}

	static void enableInterrupt(Interrupt interrupt) {
		UARTx->CR1 |= static_cast<uint32_t>(interrupt);
	}

	static void disableInterrupt(Interrupt interrupt) {
		UARTx->CR1 &= ~static_cast<uint32_t>(interrupt);
	}

	static InterruptFlag getInterruptFlags() {
//	%% if target is stm32f0 or target is stm32f3
//		return static_cast<InterruptFlag>( UARTx->ISR );
//	%% elif target is stm32f2 or target is stm32f4
		return static_cast<InterruptFlag>(UARTx->SR);
//	%% endif
	}

	static void resetInterruptFlags(InterruptFlag flags) {
//	%% if target is stm32f0 or target is stm32f3
//		// Not all flags can be cleared by writing to this reg
//		const uint32_t mask = USART_ICR_PECF  | USART_ICR_FECF   |
//			USART_ICR_NCF   | USART_ICR_ORECF | USART_ICR_IDLECF |
//			USART_ICR_TCCF  | USART_ICR_LBDCF | USART_ICR_CTSCF  |
//			USART_ICR_RTOCF | USART_ICR_EOBCF | USART_ICR_CMCF   |
//			USART_ICR_WUCF;
//		// Flags are cleared by writing a one to the flag position.
//		// Writing a zero is (hopefully) ignored.
//		UARTx->ICR = (flags & mask);
//	%% elif target is stm32f2 or target is stm32f4
		/* Interrupts must be cleared manually by accessing SR and DR.
		 * Overrun Interrupt, Noise flag detected, Framing Error, Parity Error
		 * p779: "It is cleared by a software sequence (an read to the
		 * USART_SR register followed by a read to the USART_DR register"
		 */
		if (flags & InterruptFlag::OverrunError) {
			uint32_t tmp;
			tmp = UARTx->SR;
			tmp = UARTx->DR;
			(void) tmp;
		}
		(void) flags;	// avoid compiler warning
//	%% endif
	}

	static void handleIRQ() {
		uint32_t state = UARTx->SR;

		// Read Data Register not empty
		if (state & USART_SR_RXNE)
		{
			if(rxCallback) rxCallback();
		} // USART_SR_RXNE

		// Transmit Data Register empty
		if (state & USART_SR_TC)
		{
			if(txCallback) txCallback();

		} // USART_SR_TXE
	}

private:
	static void (*txCallback)();
	static void (*rxCallback)();

	static uint32_t getClock() {
		if ((uartid == 1) || (uartid == 6)) {
			return Clocks::getPCLK2Frequency();
		} else {
			return Clocks::getPCLK1Frequency();
		}
	}
};

typedef Uart<1> Usart1;
typedef Uart<2> Usart2;
typedef Uart<3> Usart3;
typedef Uart<4> Uart4;
typedef Uart<5> Uart5;
typedef Uart<6> Usart6;

}
}


//#endif // XPCC_STM32_{{ uart | upper }}HAL_{{ id }}_HPP
