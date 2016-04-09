#include <xpcc/processing/function.hpp>
#include "../device.h"

namespace xpcc {
namespace lpc11 {

static xpcc::function<void()> irqHandler;

template<>
void Timer32B0::attachInterrupt(xpcc::function<void()> fn) {
	irqHandler = fn;
}

extern "C"
void TIMER32_0_IRQHandler() {
	irqHandler();
}
}
}
