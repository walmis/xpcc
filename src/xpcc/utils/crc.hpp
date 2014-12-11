/*
 * crc16.hpp
 *
 *  Created on: Dec 10, 2014
 *      Author: walmis
 */

#ifndef SRC_XPCC_UTILS_CRC_HPP_
#define SRC_XPCC_UTILS_CRC_HPP_

#include <stdint.h>

namespace xpcc {
class CRC {
public:
	static uint16_t crc16_ccitt(const uint8_t *buf, int len);
};
}

#endif /* SRC_XPCC_UTILS_CRC_HPP_ */
