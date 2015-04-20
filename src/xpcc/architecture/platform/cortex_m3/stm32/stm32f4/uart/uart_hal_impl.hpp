// coding: utf-8
/* Copyright (c) 2013, Roboterclub Aachen e.V.
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------
//%#
//%% if id in [1, 2, 3, 6]
//%%	set uart = "Usart"
//%% elif id in [4, 5]
//%%	set uart = "Uart"
//%% endif
//%#
//%% set name = uart ~ "Hal" ~ id
//%% set peripheral = uart | upper ~ id
//
//#ifndef XPCC_STM32_{{ uart | upper }}HAL_{{ id }}_HPP
//#	error 	"Don't include this file directly, use" \
//#			"'{{ uart | lower }}_hal_{{ id }}.hpp' instead!"
//#endif

// ----------------------------------------------------------------------------
