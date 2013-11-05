// coding: utf-8
/* Copyright (c) 2012, Roboterclub Aachen e.V.
* All Rights Reserved.
*
* The file is part of the xpcc library and is released under the 3-clause BSD
* license. See the file `LICENSE` for the full license governing this code.
*/
// ----------------------------------------------------------------------------

#ifndef XPCC_UI_TLC594X_LED_HPP
#define XPCC_UI_TLC594X_LED_HPP

#include <stdint.h>
#include "led.hpp"
#include "tables.hpp"
#include <xpcc/architecture/driver/accessor.hpp>
#include <xpcc/driver/pwm/tlc594x.hpp>


namespace xpcc
{

namespace ui
{

/**
 * LED Implementation for the TLC594X.
 *
 * @author	Niklas Hauser
 * @ingroup led
 */
template< typename PwmController >
class TLC594XLed : public Led
{
	const uint8_t channel;
	xpcc::accessor::Flash<uint16_t> table;

	virtual void
	setValue(uint8_t brightness)
	{
		PwmController::setChannel(channel, table[brightness]);
	}

public:
	TLC594XLed(const uint8_t channel, const uint16_t* table=table12_256)
	:	Led(), channel(channel), table(table)
	{
	}
};

} // namespace ui

} // namespace xpcc

#endif	// XPCC_UI_TLC594X_LED_HPP
