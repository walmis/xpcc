/*
 * uart0.cpp
 *
 *  Created on: Sep 29, 2014
 *      Author: walmis
 */

#include "uart.hpp"
#include <stdio.h>
namespace xpcc {
namespace lpc17 {

template<>
void (*Uart2::txCallback)() = 0;

template<>
void (*Uart2::rxCallback)() = 0;

extern "C"
void UART0_IRQHandler() {
	Uart2::handleIRQ();
}

}
}


