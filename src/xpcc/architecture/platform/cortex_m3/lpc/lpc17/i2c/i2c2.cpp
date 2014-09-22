/*
 * i2c2.cpp
 *
 *  Created on: Sep 20, 2014
 *      Author: walmis
 */

#include "../i2c_master.hpp"

namespace xpcc {

template <>
xpcc::I2cDelegate *lpc17::I2cMaster2::delegate = 0;

extern "C" void I2C2_IRQHandler() {
	lpc17::I2cMaster2::IRQ();
}

}



