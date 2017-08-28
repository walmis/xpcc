#include <xpcc/architecture.hpp>

#define PTR_ROM_DRIVER_TABLE ((unsigned int *)(0x1FFF1FF8))
extern "C" {
unsigned int *pDivRom_idiv;
unsigned int *pDivRom_uidiv;
}

extern "C" void __early_init() {

    SystemInit();
}

extern "C" void __late_init() {
    unsigned int *div_ptr = (unsigned int *)((unsigned int *)*(PTR_ROM_DRIVER_TABLE))[4];
    // Get addresses of integer divide routines in ROM
    // These address are then used by the code in aeabi_romdiv_patch.s
    pDivRom_idiv = (unsigned int *)div_ptr[0];
    pDivRom_uidiv = (unsigned int *)div_ptr[1];

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
