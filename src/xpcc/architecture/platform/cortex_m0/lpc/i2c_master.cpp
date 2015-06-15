/*
 * i2c_master.cpp
 *
 *  Created on: Dec 11, 2014
 *      Author: walmis
 */

#include "i2c_master.hpp"

using namespace xpcc;
using namespace xpcc::lpc11;

xpcc::I2cTransaction* lpc11::I2cMaster0::delegate = 0;
volatile bool I2cMaster0::newSession;
uint8_t * I2cMaster0::readPointer;
const uint8_t * I2cMaster0::writePointer;
std::size_t I2cMaster0::readBytesLeft;
std::size_t I2cMaster0::writeBytesLeft;
xpcc::I2c::Operation I2cMaster0::nextOperation;
volatile xpcc::I2cMaster::Error I2cMaster0::error(xpcc::I2cMaster::Error::NoError);

extern "C" void I2C_IRQHandler() {
	lpc11::I2cMaster0::IRQ();
}



