#include <xpcc/architecture.hpp>



extern "C" void __early_init() {
    SystemInit();
}

extern "C" void __user_late_init() __attribute__((weak));

extern "C" void __late_init() {
	if(__user_late_init()) __user_late_init();
    xpcc::lpc11::SysTickTimer::enable();
}


extern "C" __attribute__((noreturn, weak))
void __default_exit(void) {
  while (true) {
  }
}

extern "C"
__attribute__((weak))
void __core_init(void) { }

extern "C"
__attribute__((weak))
void __init_ram_areas() { }
    
