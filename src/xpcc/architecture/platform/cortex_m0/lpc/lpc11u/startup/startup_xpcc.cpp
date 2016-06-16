#include <xpcc/architecture.hpp>

extern "C" void startup_xpcc() {
        xpcc::lpc11::SysTickTimer::enable();
}
