/*
 * io_buffer.hpp
 *
 *  Created on: Sep 29, 2014
 *      Author: walmis
 */

#ifndef IO_BUFFER_HPP_
#define IO_BUFFER_HPP_

#include <inttypes.h>
#include <stddef.h>

class IOBuffer {

public:
	IOBuffer();
	IOBuffer(uint16_t size);
	IOBuffer(uint8_t* buf, size_t size);

	bool allocate(uint16_t size) { return _allocBuffer(size); }
	void free() { _freeBuffer(); }

	size_t bytes_free();
	size_t bytes_used();

	bool empty() { return head == tail; }

	size_t write(const uint8_t* buffer, size_t size);

	inline size_t write(uint8_t c) {
		uint16_t i = (head + 1) & mask;

		// if there isn't enough room for it in the transmit buffer
		if (i == tail) {
			return 0;
		}
		// add byte to the buffer
		bytes[head] = c;
		head = i;
		// return number of bytes written (always 1)
		return 1;
	}

	inline int16_t read(void) {
		uint16_t c;
		// if the head and tail are equal, the buffer is empty
		if ((head == tail))
			return (-1);

		// pull character from tail
		c = bytes[tail];
		tail = (tail + 1) & mask;
		return (c);
	}

	int16_t read(uint8_t* buffer, size_t size);

	void flush(void);

private:
	void _freeBuffer();
	bool _allocBuffer(uint16_t size);

	volatile uint16_t head, tail;	///< head and tail pointers
	uint16_t mask;					///< buffer size mask for pointer wrap
	uint8_t *bytes;					///< pointer to allocated buffer
};

template <int size>
class StaticIOBuffer : public IOBuffer {
public:
	StaticIOBuffer() : IOBuffer(buf, size) {}

	uint8_t buf[size];
};

#endif /* IO_BUFFER_HPP_ */
