/*
 * i2c3.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: walmis
 */

#include <xpcc/architecture/platform/cortex_m3/stm32/stm32f4/i2c/i2c_master.hpp>

namespace xpcc {
namespace stm32 {

template<>
xpcc::I2cTransaction * volatile I2cMaster3::delegate = 0;

extern "C" void
I2C3_EV_IRQHandler(void)
{
	IRQWrapper w;
	I2cMaster3::handleIRQ();
}

// ----------------------------------------------------------------------------
extern "C" void
I2C3_ER_IRQHandler(void)
{
	IRQWrapper w;
	I2cMaster3::handleERR_IRQ();
}

}
}
