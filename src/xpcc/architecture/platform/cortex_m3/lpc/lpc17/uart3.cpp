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
void (*Uart3::txCallback)() = 0;

template<>
void (*Uart3::rxCallback)() = 0;

extern "C"
void UART0_IRQHandler() {
	Uart3::handleIRQ();
}

}
}


