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

		Uart::init(baud);

		Uart::attachTxCompleteInterrupt(std::bind(&BufferedUart::onTxComplete, this));
		Uart::attachRxCompleteInterrupt(std::bind(&BufferedUart::onRxComplete, this));

		Uart::enableTxCompleteInterrupt(true);
		Uart::enableRxCompleteInterrupt(true);

	}

	void write(char c) {

		if(Uart::txEmpty()) {
			Uart::write(c);
		} else {
			XPCC_LOG_DEBUG .printf("b\n");
			BufferedIODevice::write(c);
		}
	}

private:
	void onTxComplete() {
		XPCC_LOG_DEBUG .printf("tx %d\n", txbuf.bytes_used());
	}

	void onRxComplete() {
		XPCC_LOG_DEBUG .printf("rx\n");
	}

};

}
}

#endif /* BUFFERED_UART_HPP_ */
