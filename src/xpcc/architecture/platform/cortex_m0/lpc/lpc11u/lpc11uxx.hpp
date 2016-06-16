/*
 * lpc11uxx.hpp
 *
 *  Created on: May 3, 2013
 *      Author: walmis
 */


#include <lpc11xx/cmsis/LPC11Uxx.h>
#include <lpc11xx/cmsis/core_cm0.h>
#include <lpc11xx/cmsis/system_LPC11xx.h>

#include <xpcc/architecture/utils.hpp>

// Chapter 12: General Purpose I/O (GPIO)
#include "../iocon.hpp"
#include "gpio.hpp"

#include "gpio_interrupt.h"

// Chapter 13: UART
#include "../uart/uart_1.hpp"

#include "spi/spi_0.hpp"
#include "spi/spi_1.hpp"

#include "usb/USBEndpoints_LPC11U.h"


#include "../iap.hpp"

#include "eeprom.hpp"

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

#include "timer.hpp"

#include "watchdog.hpp"


