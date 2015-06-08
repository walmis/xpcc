/*
 * i2c2.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: walmis
 */

#include "i2c_master.hpp"
namespace xpcc {
namespace stm32 {
template<>
xpcc::I2cTransaction * volatile I2cMaster2::delegate = 0;

extern "C" void
I2C2_EV_IRQHandler(void)
{
	IRQWrapper w;
	I2cMaster2::handleIRQ();
}

// ----------------------------------------------------------------------------
extern "C" void
I2C2_ER_IRQHandler(void)
{
	IRQWrapper w;
	I2cMaster2::handleERR_IRQ();
}

}
}
