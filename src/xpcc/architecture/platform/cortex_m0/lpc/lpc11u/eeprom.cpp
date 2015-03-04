/*
 * eeprom.cpp
 *
 *  Created on: Mar 3, 2015
 *      Author: walmis
 */

#include <stdint.h>
#include "eeprom.hpp"
#include "../iap.hpp"


namespace xpcc {
namespace lpc11u {

bool EEPROM::write(uint8_t* eeAddress, uint8_t* buffAddress, uint32_t byteCount ) {
	uint32_t command[5], result[5];

	command[0] = 61;
	command[1] = (uint32_t) eeAddress;
	command[2] = (uint32_t) buffAddress;
	command[3] = byteCount;
	command[4] = SystemCoreClock/1000;

	/* Invoke IAP call...*/
	lpc11::IAP::call(command, result);

	return result[0] == 0;
}
bool EEPROM::read(uint8_t* eeAddress, uint8_t* buffAddress, uint32_t byteCount ) {
	uint32_t command[5], result[5];

	command[0] = 62;
	command[1] = (uint32_t) eeAddress;
	command[2] = (uint32_t) buffAddress;
	command[3] = byteCount;
	command[4] = SystemCoreClock/1000;

	/* Invoke IAP call...*/
  	lpc11::IAP::call( command, result);

	return result[0] == 0;
}

}
}
