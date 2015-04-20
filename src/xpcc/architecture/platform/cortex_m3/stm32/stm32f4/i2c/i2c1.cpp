/*
 * i2c1.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: walmis
 */

#include "i2c_master.hpp"

namespace xpcc {
namespace stm32 {

template<>
xpcc::I2cDelegate * volatile I2cMaster1::delegate = 0;


extern "C" void
I2C1_EV_IRQHandler(void)
{
	I2cMaster1::handleIRQ();
}

// ----------------------------------------------------------------------------
extern "C" void
I2C1_ER_IRQHandler(void)
{
	I2cMaster1::handleERR_IRQ();
}

}
}
