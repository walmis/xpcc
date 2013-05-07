/*
 * lpc11uxx.hpp
 *
 *  Created on: May 3, 2013
 *      Author: walmis
 */

#ifndef LPC11UXX_HPP_
#define LPC11UXX_HPP_

#include <lpc11xx/cmsis/LPC11Uxx.h>
#include <lpc11xx/cmsis/core_cm0.h>
#include <lpc11xx/cmsis/system_LPC11xx.h>

// Chapter 12: General Purpose I/O (GPIO)
#include "gpio.hpp"
#include "../iocon.hpp"

#include "spi/spi_0.hpp"
#include "spi/spi_1.hpp"

#include "usb/USBEndpoints_LPC11U.h"

#include "gpio_interrupt.h"

// Chapter 13: UART
#include "../uart/uart_1.hpp"

// Chapter 24: LPC111x/LPC11Cxx System tick timer (SysTick)
#if defined(__ARM_LPC11UXX__) || defined(__ARM_LPC11XX__) || defined(__ARM_LPC11CXX__)
#include "../systick_timer.hpp"
#endif

// Chapter 25: LPC111x/LPC11Cxx ADC
#if defined(__ARM_LPC11UXX__) || defined(__ARM_LPC11XX__) || defined(__ARM_LPC11CXX__)
//#include "adc/adc.hpp"
#endif

// Chapter 26: LPC111x/LPC11Cxx Flash programming firmware
#if defined(__ARM_LPC11UXX__) || defined(__ARM_LPC11XX__) || defined(__ARM_LPC11CXX__)
#include "../rand.hpp"
#endif


#endif /* LPC11UXX_HPP_ */
