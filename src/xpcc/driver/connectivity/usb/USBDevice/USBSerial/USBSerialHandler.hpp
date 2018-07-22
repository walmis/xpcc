/*
 * USBSerialHandler.hpp
 *
 *  Created on: Oct 13, 2013
 *      Author: walmis
 */

#ifndef USBSERIALHANDLER_HPP_
#define USBSERIALHANDLER_HPP_

#include "../USBDevice/USBDevice.h"
#include "../USBDevice/USBInterfaceHandler.h"
#include <xpcc/container.hpp>
#include <xpcc/processing.hpp>
#include <xpcc/io/iodevice.hpp>
#include "USBCDC.h"

namespace xpcc {

#define DEFAULT_CONFIGURATION (1)

#define CDC_SET_LINE_CODING        0x20
#define CDC_GET_LINE_CODING        0x21
#define CDC_SET_CONTROL_LINE_STATE 0x22

#define MAX_CDC_REPORT_SIZE MAX_PACKET_SIZE_EPBULK

class USBSerialHandler : public USBInterfaceHandler, public IODevice {
public:

	USBSerialHandler(uint8_t bulkIn = CDC_EPBULK_IN,
			uint8_t bulkOut = CDC_EPBULK_OUT, uint8_t intIn = CDC_EPINT_IN) :
			bulkIn(bulkIn), bulkOut(bulkOut), intIn(
					intIn), latency_timer(){

		inEp_request = true;
		data_waiting = false;
		terminal_connected = false;
		isActive = true;

		//XPCC_LOG_DEBUG .printf("SerialHandler dev:%x, %d %d %d\n", device, bulkIn, bulkOut, intIn);

	}

	void setLatency(uint8_t latency) {
		this->latency = latency;
	}

	int16_t rxAvailable();
	int16_t txAvailable();

    void putch(char c);
	int16_t getch();

	//IODevice overrides
	size_t
	write(char c) override {
		putch(c);
		return 1;
	}

	void flush() override;

	/// Read a single character
	int16_t	read() override {
		return getch();
	}
	///////

protected:
	void SOF(int frameNumber) override;
	bool USBCallback_request(void) override;
	bool USBCallback_setConfiguration(uint8_t configuration) override;
	void USBCallback_requestCompleted(uint8_t * buf, uint32_t length) override;

	uint8_t cdc_line_coding[7]= {0x80, 0x25, 0x00, 0x00, 0x00, 0x00, 0x08};

private:
	//DoubleBuffer<MAX_CDC_REPORT_SIZE> rx_buffer;

	bool EP_handler(uint8_t ep) override;

	void sendPacket();

	uint8_t latency = 32;

	uint8_t bulkIn;
	uint8_t bulkOut;
	uint8_t intIn;

	volatile bool data_waiting;
	volatile bool inEp_request;
	volatile bool isActive;

	volatile bool terminal_connected;

	bool send(uint8_t * buffer, uint32_t size);
	bool readEP(uint8_t * buffer, uint32_t * size);
	bool readEP_NB(uint8_t * buffer, uint32_t * size);

	xpcc::Timeout<> latency_timer;

	StaticIOBuffer<256> tx_buffer;
	StaticIOBuffer<128> rx_buffer;
	Event event;
};

}



#endif /* USBSERIALHANDLER_HPP_ */
