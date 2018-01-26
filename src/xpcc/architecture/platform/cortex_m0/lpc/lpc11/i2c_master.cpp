
#include "../i2c_master.hpp"

namespace xpcc {
namespace lpc11 {

volatile bool I2cMaster::newSession;
uint8_t * I2cMaster::readPointer;
const uint8_t * I2cMaster::writePointer;
std::size_t I2cMaster::readBytesLeft;
std::size_t I2cMaster::writeBytesLeft;
xpcc::I2c::Operation I2cMaster::nextOperation;
// delegating
xpcc::I2cTransaction *I2cMaster::delegate;
volatile xpcc::I2cMaster::Error I2cMaster::error(xpcc::I2cMaster::Error::NoError);

}
}


extern "C" void
I2C_IRQHandler() {
	xpcc::IRQWrapper w;
	xpcc::lpc11::I2cMaster::IRQ();
}
