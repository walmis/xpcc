/*
 * i2c1.cpp
 *
 *  Created on: Sep 20, 2014
 *      Author: walmis
 */


#include "../i2c_master.hpp"

namespace xpcc {

template <>
xpcc::I2cTransaction* volatile lpc17::I2cMaster1::delegate = 0;

extern "C" void I2C1_IRQHandler() {
	lpc17::I2cMaster1::IRQ();
}

}


