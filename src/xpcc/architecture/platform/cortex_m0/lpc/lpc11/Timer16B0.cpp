/*
 * Timer16B0.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: walmis
 */

#include <xpcc/processing/function.hpp>
#include "../device.h"

namespace xpcc {
namespace lpc11 {

static xpcc::function<void()> irqHandler;

template<>
void Timer16B0::attachInterrupt(xpcc::function<void()> fn) {
	irqHandler = fn;
}

extern "C"
void TIMER16_0_IRQHandler() {
	irqHandler();
}
}
}




