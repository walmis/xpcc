/* Copyright (c) 2013, Valmantas Palikša <walmis@balticum-tv.lt>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Roboterclub Aachen e.V. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef FRAME_H_
#define FRAME_H_

#include <stdint.h>
#include <string.h>
#include <xpcc/debug.hpp>

inline void putDword(uint8_t* address, uint32_t dword) {
	memcpy(address, (void*)&dword, sizeof(uint32_t));
}

inline uint32_t getDword(uint8_t* address) {
	uint32_t tmp;
	memcpy((void*)&tmp, address, sizeof(uint32_t));
	return tmp;
}

inline void putWord(uint8_t* address, uint16_t word) {
	*(address+1) = word >> 8;
	*(address) = word;
}

inline uint16_t getWord(uint8_t* address) {
	return (uint16_t)((*address+1 << 8) | *(address));
}

class Frame {
public:

	Frame(uint8_t size = 0) {
		data = 0;
		rx_flag = 0;
		data_len = 0;
		lqi = 0;
		buffer_size = 0;
		if(size)
			allocate(size);
	}

	virtual ~Frame() {
		if(data) {
			delete[] data;
		}
	}

	bool allocate(uint8_t len = 127) {
		if(data)
			delete[] data;

		data = new uint8_t[len];

		if(data) {
			memset(data, 0, len);
			buffer_size = len;
			return true;
		} else {
			buffer_size = 0;
			return false;
		}
	}

	uint8_t lqi;
	uint8_t buffer_size;
	uint8_t data_len;
	uint8_t rx_flag;

	uint8_t *data;
};




class StaticFrame : public Frame {
public:
	StaticFrame() {
		data = _data;
		buffer_size = 127;
		data_len = 0;
		memset(_data, 0, 127);
	}

	bool allocate(uint8_t len) {
		return false;
	}

	~StaticFrame() {
		data = 0;
	}
	uint8_t _data[127];
};


#endif /* FRAME_H_ */
