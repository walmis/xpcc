#include <xpcc/processing/timer_registry.hpp>

#include "device.h"


TimerRegistry::TimerRegistry() {
    xpcc::lpc11::SysTickTimer::attachInterrupt(TimerRegistry::_timerTick);
}