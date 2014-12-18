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

	~IOBuffer();

	size_t bytes_free();
	size_t bytes_used();

	size_t write(const uint8_t* buffer, size_t size);
	size_t write(uint8_t c);
	int16_t read(void);

	void flush(void);

private:
	void _freeBuffer();
	bool _allocBuffer(uint16_t size);

	volatile uint16_t head, tail;	///< head and tail pointers
	uint16_t mask;					///< buffer size mask for pointer wrap
	uint8_t *bytes;					///< pointer to allocated buffer
};

#endif /* IO_BUFFER_HPP_ */
