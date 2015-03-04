/*
 * eeprom.hpp
 *
 *  Created on: Mar 3, 2015
 *      Author: walmis
 */

#ifndef EEPROM_HPP_
#define EEPROM_HPP_

namespace xpcc {
namespace lpc11u {


class EEPROM {
public:
	static bool write(uint8_t* eeAddress, uint8_t* buffAddress, uint32_t byteCount );
	static bool read(uint8_t* eeAddress, uint8_t* buffAddress, uint32_t byteCount );
};

}
}



#endif /* EEPROM_HPP_ */
