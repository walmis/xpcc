/*
 * stringstream.hpp
 *
 *  Created on: Sep 4, 2014
 *      Author: walmis
 */

#ifndef STRINGSTREAM_HPP_
#define STRINGSTREAM_HPP_

#include <xpcc/io.hpp>

namespace xpcc {

template <uint8_t N>
class StringStream : public xpcc::IOStream, xpcc::IODevice {
public:
	StringStream() : xpcc::IOStream(*static_cast<xpcc::IODevice*>(this)) {
		clear();
	}

	char buffer[N];
	uint8_t pos;

	void clear() {
		pos = 0;
	}

	////----IODevice overrides----//////
	size_t write(char c) {
		if(pos < N) {
			buffer[pos++] = c;
			buffer[pos] = 0;
			return 1;
		}
		return 0;
	}

	int16_t read() {
		return -1;
	}

	void flush() {}
};

}

#endif /* STRINGSTREAM_HPP_ */
