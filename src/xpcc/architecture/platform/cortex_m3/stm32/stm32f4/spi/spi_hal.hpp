// coding: utf-8
/* Copyright (c) 2013, Roboterclub Aachen e.V.
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef XPCC_STM32_SPI_HAL_HPP
#define XPCC_STM32_SPI_HAL_HPP

#include "spi_base.hpp"

namespace xpcc
{

namespace stm32
{

/**
 * Serial peripheral interface (SPI{{ id }})
 *
 * Very basic implementation that exposes more hardware features than
 * the regular Spi classes.
 *
 * @author		Kevin Laeufer
 * @ingroup		stm32
 */
template <int spiid>
class SpiHal : public SpiBase
{
public:
	/// Enables the clock, resets the hardware and sets the SPE bit
	static void
	enable();

	/// Disables the hw module (by disabling its clock line)
	static void
	disable();

	/**
	 * Initialize Spi Peripheral
	 *
	 * Enables clocks, the UART peripheral
	 * Sets baudrate and parity.
	 */
	static void
	initialize(	Prescaler prescaler,
				MasterSelection masterSelection = MasterSelection::Master,
				DataMode dataMode   = DataMode::Mode0,
				DataOrder dataOrder = DataOrder::MsbFirst,
				DataSize  dataSize  = DataSize::Bit8);

	static void
	setDataMode(DataMode dataMode);

	static void
	setDataOrder(DataOrder dataOrder);

	static void
	setDataSize(DataSize dataSize);

	static void
	setMasterSelection(MasterSelection masterSelection);

	/// Returns true if data has been received
	static bool
	isReceiveRegisterNotEmpty();

	/// Returns true if data can be written
	static bool
	isTransmitRegisterEmpty();

	/**
	 * Write up to 16 Bit to the data register
	 *
	 * @warning 	This method does NOT do any sanity checks!!
	 *				It is your responsibility to check if the register
	 *				is empty!
	 */
	static void
	write(uint16_t data);

	/**
	 * Returns the value of the data register
	 *
	 * @warning 	This method does NOT do any sanity checks!!
	 *				It is your responsibility to check if the register
	 *				contains something useful!
	 */
	static void
	read(uint8_t &data);

	/**
	 * Returns the value of the data register
	 *
	 * @warning 	This method does NOT do any sanity checks!!
	 *				It is your responsibility to check if the register
	 *				contains something useful!
	 */
	static void
	read(uint16_t &data);

	static void
	enableInterruptVector(bool enable, uint32_t priority);

	static void
	enableInterrupt(Interrupt interrupt);

	static void
	disableInterrupt(Interrupt interrupt);

	static InterruptFlag
	getInterruptFlags();

	/**
	 * 
	 *
	 * @warning 	Not all InterruptFlags can be cleared this way.
	 */
	static void
	acknowledgeInterruptFlag(InterruptFlag flags);
};

typedef SpiHal<1> Spi1;
typedef SpiHal<2> Spi2;
typedef SpiHal<3> Spi3;

} // namespace stm32

} // namespace xpcc



#include "spi_hal_impl.hpp"

#endif // XPCC_STM32_SPI_HAL{{ id }}_HPP
