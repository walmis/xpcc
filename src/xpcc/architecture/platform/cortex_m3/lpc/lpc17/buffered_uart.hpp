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

	size_t write(char c) {
		if(Uart::txEmpty()) {
			Uart::write(c);
			return 1;
		} else {
			//XPCC_LOG_DEBUG .printf("b %d\n");
			while(BufferedIODevice::write(c) == 0) {};
			return 1;
		}

	}

private:
	static BufferedUart* inst;
	static void onTxComplete() {
		if(!inst) return;
		int16_t ch = inst->txbuf.read();
		if(ch >= 0)
			Uart::write(ch);
	}

	static void onRxComplete() {
		if(!inst) return;
		while(!Uart::rxEmpty()) {
			uint8_t ch = 0;
			Uart::read(ch);
			inst->rxbuf.write(ch);
		}
	}

};

template <typename Uart>
BufferedUart<Uart>* BufferedUart<Uart>::inst = 0;

}
}

#endif /* BUFFERED_UART_HPP_ */
