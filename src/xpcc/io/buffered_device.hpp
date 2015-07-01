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
#include <xpcc/architecture/driver/atomic.hpp>
#include <xpcc/processing.hpp>

class BufferedIODevice : public xpcc::IODevice {
public:
	BufferedIODevice(uint16_t txS, uint16_t rxS) :
	txbuf(txS), rxbuf(rxS)
	{

	}

	virtual size_t write(char c) {
		while(_blocking && !txAvailable()) txEvent.wait(1);
		return txbuf.write(c);
	}

	virtual size_t write(const uint8_t* buf, size_t len) {
		if(_blocking) {
			while(len) {
				int16_t wr = std::min((int16_t)len, txAvailable());
				if(wr<1) {
					txEvent.wait(1);
					continue;
				}
				txbuf.write(buf, wr);
				len-=wr;
				buf+=wr;
			}
			return len;
		}
		return txbuf.write(buf, len);
	}

	/// Read a single character
	virtual int16_t read() {
		return rxbuf.read();
	}

	virtual void flush() {
	}

	//returns available bytes in the rx buffer
	virtual int16_t rxAvailable() {
		return rxbuf.bytes_used();
	}

	//returns available bytes in tx buffer
	virtual int16_t txAvailable() {
		return txbuf.bytes_free();
	}

protected:

	int16_t popTx() {
		int16_t c = txbuf.read();
		txEvent.signal();
		return c;
	}


	IOBuffer txbuf;
	IOBuffer rxbuf;

	xpcc::Event txEvent;
};

#endif /* BUFFERED_DEVICE_HPP_ */
