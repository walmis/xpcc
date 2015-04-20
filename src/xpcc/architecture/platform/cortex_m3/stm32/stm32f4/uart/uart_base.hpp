// coding: utf-8
/*
* Copyright (c) 2013, Roboterclub Aachen e.V.
* All Rights Reserved.
*
* The file is part for the xpcc library and is released under the
* 3-clause BSD license. See the file `LICENSE` for the full license
* governing this code.
*/
// ----------------------------------------------------------------------------

#ifndef XPCC_STM32_UART_BASE_HPP
#define XPCC_STM32_UART_BASE_HPP

#include <stdint.h>
#include "../../../stm32.hpp"

namespace xpcc
{
namespace stm32
{
/**
 * Base class for the UART classes
 *
 * Provides some common enum that do not depend on the specific UART.
 *
 * @author Kevin Laeufer
 * @internal
 * @ingroup		stm32
 */
class UartBase
{

public:
	enum class Interrupt : uint32_t
	{
//%% if target is stm32f0 or target is stm32f3
		/// A USART interrupt is generated when match character is received.
//		CharacterMatch = USART_CR1_CMIE,
//%% endif
		/// Call interrupt when a parity error occurred.
		ParityError	= USART_CR1_PEIE,
		/// Call interrupt when transmit register is empty (i.e. the byte has been transfered to the shift out register
		TxEmpty		= USART_CR1_TXEIE,
		/// Called when the byte was completely transmitted
		TxComplete	= USART_CR1_TCIE,
		/// Call interrupt when char received (RXNE) or overrun occurred (ORE)
		RxNotEmpty	= USART_CR1_RXNEIE,
	};

	enum class InterruptFlag : uint32_t
	{
//%% if target is stm32f0 or target is stm32f3
//%%	set reg = "ISR"
		/// Set if match character is received.
//		CharacterMatch	= USART_{{reg}}_CMF,
//%% elif target is stm32f2 or target is stm32f4
//%%	set reg = "SR"
//%% endif
		/// Set if the transmit data register is empty.
		TxEmpty			= USART_SR_TXE,
		/// Set if the transmission is complete.
		TxComplete		= USART_SR_TC,
		/// Set if the receive data register is not empty.
		RxNotEmpty		= USART_SR_RXNE,
		/// Set if receive register was not cleared.
		OverrunError	= USART_SR_ORE,
		/// Set if a de-synchronization, excessive noise or a break character is detected
		FramingError 	= USART_SR_FE,
		/// Set if a parity error was detected.
		ParityError		= USART_SR_PE,
	};

	enum class Parity : uint32_t
	{
		Disabled 	= 0,
		Even 		= USART_CR1_PCE,
		Odd  		= USART_CR1_PCE | USART_CR1_PS,
	};

	/// Spi Data Mode, Mode0 is the most common mode
	enum class
	SpiDataMode : uint32_t
	{
		Mode0 = 0b00,			///< clock normal,   sample on rising  edge
		Mode1 = USART_CR2_CPHA,	///< clock normal,   sample on falling edge
		Mode2 = USART_CR2_CPOL,	///< clock inverted, sample on falling  edge
		Mode3 = USART_CR2_CPOL | USART_CR2_CPHA
		///< clock inverted, sample on rising edge
	};

	enum class
	OversamplingMode : uint32_t
	{
		By16 = 0b0,
		By8  = USART_CR1_OVER8,
	};

	enum class
	LastBitClockPulse : uint32_t
	{
		DoNotOutput = 0b0,
		Output = USART_CR2_LBCL,
	};

	enum class
	SpiClock : uint32_t
	{
		Disabled = 0b0,
		Enabled  = USART_CR2_CLKEN,
	};

};
ENUM_CLASS_FLAG(UartBase::Interrupt)
ENUM_CLASS_FLAG(UartBase::InterruptFlag)
}
}

#endif // XPCC_STM32_UART_BASE_HPP
