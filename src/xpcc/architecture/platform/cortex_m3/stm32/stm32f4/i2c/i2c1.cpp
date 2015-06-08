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
xpcc::I2cTransaction * volatile I2cMaster1::delegate = 0;


extern "C" void
I2C1_EV_IRQHandler(void)
{
	IRQWrapper w;
	I2C1->CR2 &= ~I2C_CR2_ITERREN;
	I2cMaster1::handleIRQ();
	I2C1->CR2 |= I2C_CR2_ITERREN;
}

// ----------------------------------------------------------------------------
extern "C" void
I2C1_ER_IRQHandler(void)
{
	IRQWrapper w;
	I2C1->CR2 &= ~I2C_CR2_ITEVTEN;
	I2cMaster1::handleERR_IRQ();
	I2C1->CR2 |= I2C_CR2_ITEVTEN;
}

}
}
