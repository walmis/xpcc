/* Copyright (c) 2010-2011 mbed.org, MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "stdint.h"
#include "USBSerial.h"

using namespace xpcc::lpc17;
#include <lpc17xx/cmsis/LPC17xx.h>
void xpcc::lpc17::USBSerial::write(char c) {

	tx_buffer.push(c);

	//check that we are in thread mode
	if (tx_buffer.isNearlyFull() && __get_IPSR() == 0) {
		uint8_t buf[64];
		int size = 0;

		for (int i = 0; i < 64; i++) {
			buf[i] = tx_buffer.get();
			size++;

			tx_buffer.pop();
			if (tx_buffer.isEmpty())
				break;
		}
		send(buf, size);

	} else {
		in_request = true;
	}

}


bool USBSerial::EP2_OUT_callback() {
    uint8_t c[65];
    uint32_t size = 0;

    //we read the packet received and put it on the circular buffer

	if (rx_buffer.free() >= 64) {
		readEP(c, &size);
		for (uint32_t i = 0; i < size; i++) {
			rx_buffer.push(c[i]);
		}
		return true;
	}
	data_waiting = true;
	return true;

}

uint8_t USBSerial::available() {
	return rx_buffer.stored();
}


bool xpcc::lpc17::USBSerial::read(char& c) {

	if(rx_buffer.isEmpty()) return false;
	c = rx_buffer.get();
	rx_buffer.pop();
	return true;
}

bool xpcc::lpc17::USBSerial::EP2_IN_callback() {
	in_request = true;

	return false;
}

void xpcc::lpc17::USBSerial::SOF(int frameNumber) {
	uint8_t buf[64];
	uint32_t size = 0;

	if(!tx_buffer.isEmpty() && in_request) {

		for(int i=0; i<64; i++) {
			buf[i] = tx_buffer.get();
			size++;

			tx_buffer.pop();
			if(tx_buffer.isEmpty()) break;
		}

		writeNB(EPBULK_IN, buf, size, 64);
		in_request = false;
	}

	if(data_waiting && rx_buffer.free() >= 64) {
		if(readEP_NB(buf, &size)) {
			for (uint32_t i = 0; i < size; i++) {
				rx_buffer.push(buf[i]);
			}
			data_waiting = false;
		}
	}

}
