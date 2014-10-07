/*
 * buffered_uart.hpp
 *
 *  Created on: Sep 29, 2014
 *      Author: walmis
 */

#ifndef BUFFERED_UART_HPP_
#define BUFFERED_UART_HPP_

#include "uart.hpp"
#include <xpcc/io/buffered_device.hpp>

namespace xpcc{
namespace lpc17 {

template <typename Uart>
class BufferedUart : public BufferedIODevice {
public:
	BufferedUart(uint32_t baud, uint16_t txS, uint16_t rxS) :
		BufferedIODevice(txS, rxS) {
		inst = this;

		Pinsel::setFunc(0, 2, 1);
		Pinsel::setFunc(0, 3, 1);

		Uart::init(baud);

		Uart::attachTxCompleteInterrupt(onTxComplete);
		Uart::attachRxCompleteInterrupt(onRxComplete);

		Uart::enableTxCompleteInterrupt(true);
		Uart::enableRxCompleteInterrupt(true);

	}

	~BufferedUart() {
		if(inst == this)
			inst = 0;
	}
	size_t write(const uint8_t* buf, size_t len) {
		size_t n;
		if((n = BufferedIODevice::write(buf, len))) {
			if(!Uart::txBusy()) {
				_send();
			}
		}
	}
	size_t write(char c) {
		if(BufferedIODevice::write(c) > 0) {
			if(!Uart::txBusy()) {
				XPCC_LOG_DEBUG .printf("first wr %d %d %d\n", txAvailable(), LPC_UART0->LSR & (1<<6),  LPC_UART0->LSR & (1<<5));

				_send();

				return 1;
			} else {
				return 1;
			}
		}
		return 0;
	}


private:
	static BufferedUart* inst;

	bool _send() {
		int16_t ch = inst->txbuf.read();
		if(ch < 0) return false;

		Uart::put(ch);
		return true;
	}

	static void onTxComplete() {
		if(!inst) return;

		if(Uart::txEmpty()) {
			//fill uart fifo
			for(int i = 0; i < 16; i++) {
				if(!inst->_send()) return;
			}
		}
	}

	static void onRxComplete() {
		if(!inst) return;
		while(!Uart::rxEmpty()) {
			inst->rxbuf.write(Uart::get());
		}
	}

};

template <typename Uart>
BufferedUart<Uart>* BufferedUart<Uart>::inst = 0;

}
}

#endif /* BUFFERED_UART_HPP_ */
