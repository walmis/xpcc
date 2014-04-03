// coding: utf-8
/* Copyright (c) 2011, Roboterclub Aachen e.V.
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#pragma once

#include "../../../device.hpp"
#include <xpcc/architecture/peripheral/i2c.hpp>
#include "type_ids.hpp"

namespace xpcc
{

namespace lpc17
{

/**
 * I2cMaster implementation of I2C{{ id }} module.
 *
 * Interrupts must be enabled.
 *
 * @author		Georgi Grinshpun
 * @author		Niklas Hauser
 * @ingroup		stm32
 */
class I2cMaster : ::xpcc::I2cMaster
{
public:
//	static const TypeId::I2cMaster{{ id }}Sda Sda;
//	static const TypeId::I2cMaster{{ id }}Scl Scl;

public:
	/**
	 * Set up the I2C module for master operation.
	 *
	 * @param	rate
	 *		`Standard` or `Fast`, `High` datarate is not supported
	 */
	template<class clockSource, DataRate rate=DataRate::Standard>
	static ALWAYS_INLINE void
	initialize()
	{
//		constexpr int peripheralFrequency = clockSource::I2c{{ id }};
//		constexpr uint32_t rawRate = static_cast<uint32_t>(rate);
//		static_assert(rawRate <= 400000, "The STM32 does not support High I2C baudrate.");
//		static_assert(peripheralFrequency/2 >= rawRate, "The APB1 frequency needs to be at least 2x higher than I2C baudrate.");
//
//		// set the prescaler rate register
//		// ccrPrescaler = peripheralFrequency / (2 * I2CFrequency)
//		constexpr uint16_t prescaler = peripheralFrequency / (2 * rawRate);
//
//		reset();
//		// Enable clock
//		RCC->APB1ENR |= RCC_APB1ENR_I2C{{ id }}EN;
//
//		I2C{{ id }}->CR1 = I2C_CR1_SWRST; 		// reset module
//		I2C{{ id }}->CR1 = 0;
//
//		NVIC_EnableIRQ(I2C{{ id }}_ER_IRQn);
//		NVIC_EnableIRQ(I2C{{ id }}_EV_IRQn);
//
//		I2C{{ id }}->CR2 = static_cast<uint32_t>(peripheralFrequency) / 1000000;
//		I2C{{ id }}->CCR = prescaler;
//		// DEBUG_STREAM("ccrPrescaler=" << prescaler);
//		I2C{{ id }}->TRISE = 0x09;
//
//		I2C{{ id }}->CR1 |= I2C_CR1_PE; // Enable peripheral
	}

	static bool
	start(xpcc::I2cDelegate *delegate);

	static bool
	startBlocking(xpcc::I2cDelegate *delegate);

	static Error
	getErrorState();

	static void
	reset(DetachCause cause=DetachCause::SoftwareReset);
	// end documentation inherited

private:
	static void
	initializeWithPrescaler(uint16_t baud);
};

}

} // namespace xpcc

