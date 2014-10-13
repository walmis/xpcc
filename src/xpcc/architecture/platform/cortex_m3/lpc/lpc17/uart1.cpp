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
void (*Uart1::txCallback)() = 0;

template<>
void (*Uart1::rxCallback)() = 0;

extern "C"
void UART1_IRQHandler() {
	Uart1::handleIRQ();
}

}
}


