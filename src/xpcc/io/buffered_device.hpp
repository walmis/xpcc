/*
 * buffered_device.hpp
 *
 *  Created on: Sep 29, 2014
 *      Author: walmis
 */

#ifndef BUFFERED_DEVICE_HPP_
#define BUFFERED_DEVICE_HPP_

#include <inttypes.h>
#include <xpcc/io/iodevice.hpp>
#include <xpcc/container/io_buffer.hpp>

class BufferedIODevice : public xpcc::IODevice {
public:
	BufferedIODevice(uint16_t txS, uint16_t rxS) :
	txbuf(txS), rxbuf(rxS)
	{

	}

	virtual void write(char c) {
		txbuf.write(c);
	}
	/// Read a single character
	virtual bool read(char& c) {
		int16_t ch = rxbuf.read();
		if(ch < 0) return false;
		c = ch;
		return true;
	}
	virtual void flush() {
	}

	virtual int16_t rxAvailable() {
		return rxbuf.bytes_used(); //unknown
	}

	virtual int16_t txAvailable() {
		return txbuf.bytes_free();
	}

protected:
	IOBuffer txbuf;
	IOBuffer rxbuf;
};

#endif /* BUFFERED_DEVICE_HPP_ */
