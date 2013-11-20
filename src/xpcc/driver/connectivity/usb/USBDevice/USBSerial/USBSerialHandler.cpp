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
#include "USBSerialHandler.hpp"

using namespace xpcc;

void USBSerialHandler::sendPacket(bool blocking) {
	isActive = true;

	uint8_t buf[64];
	int size = 0;

	for (int i = 0; i < 64; i++) {
		buf[i] = tx_buffer.get();
		size++;

		tx_buffer.pop();
		if (tx_buffer.isEmpty())
			break;
	}

	if(blocking)
		device->write(bulkIn, buf, size, MAX_CDC_REPORT_SIZE);
	else
		device->writeNB(bulkIn, buf, size, MAX_CDC_REPORT_SIZE);

	latency_timer.restart(LATENCY);
}

void USBSerialHandler::putc(char c) {

	if(tx_buffer.isFull()) {
		if(!isActive) {
			return;
		} else {

			Timeout<> t(5);

			while(tx_buffer.isFull()) {
				if(!device->configured() || t.isExpired()) {
					isActive = false;
					return;
				}
			}
		}
	}

	tx_buffer.push(c);
}


bool USBSerialHandler::EP_handler(uint8_t ep) {
    //XPCC_LOG_DEBUG .printf("ep handler %d\n", ep);

    if(ep == bulkOut) {

    	uint8_t c[64];
		uint32_t size = 0;

		//we read the packet received and put it on the circular buffer


		if (rx_buffer.free() >= 64) {
			readEP(c, &size);
			//XPCC_LOG_DEBUG .printf("bulkout %d\n", size);
			for (uint32_t i = 0; i < size; i++) {
				rx_buffer.push(c[i]);
			}
			return true;
		} else {
			data_waiting = true;
			return false;
		}

    } else

    if(ep == bulkIn) {

    	if(!tx_buffer.isEmpty()) {
    		//XPCC_LOG_DEBUG .printf("bulkIN send\n");
    		sendPacket(false);
    		return true;
    	}
    	in_request = true;
    	return false;
    }

    return false;

}

uint8_t USBSerialHandler::available() {
	return rx_buffer.stored();
}


bool USBSerialHandler::getc(char& c) {

	if(rx_buffer.isEmpty()) return false;
	c = rx_buffer.get();
	rx_buffer.pop();
	return true;
}

//bool USBSerialHandler::EP2_IN_callback() {
//	in_request = true;
//
//	return false;
//}

void USBSerialHandler::SOF(int frameNumber) {

	if(device->configured()) {

		if (!tx_buffer.isEmpty() && in_request
				&& (latency_timer.isExpired() || tx_buffer.stored() >= MAX_PACKET_SIZE_EPBULK)) {

			in_request = false;
			//XPCC_LOG_DEBUG .printf("SOF send\n");
			sendPacket(false);

		}

		if(data_waiting && rx_buffer.free() >= MAX_PACKET_SIZE_EPBULK) {
			uint8_t buf[64];
			uint32_t size;

			if(readEP_NB(buf, &size)) {

				//XPCC_LOG_DEBUG .printf("SOF Read %d\n", size);
				for (uint32_t i = 0; i < size; i++) {
					rx_buffer.push(buf[i]);
				}

				data_waiting = false;
			}

		}

	}
}

bool USBSerialHandler::send(uint8_t * buffer, uint32_t size) {
    return device->write(bulkIn, buffer, size, MAX_CDC_REPORT_SIZE);
}

bool USBSerialHandler::readEP(uint8_t * buffer, uint32_t * size) {
    if (!device->readEP(bulkOut, buffer, size, MAX_CDC_REPORT_SIZE))
        return false;
    if (!device->readStart(bulkOut, MAX_CDC_REPORT_SIZE))
        return false;
    return true;
}

bool USBSerialHandler::readEP_NB(uint8_t * buffer, uint32_t * size) {
    if (!device->readEP_NB(bulkOut, buffer, size, MAX_CDC_REPORT_SIZE))
        return false;
    if (!device->readStart(bulkOut, MAX_CDC_REPORT_SIZE))
        return false;
    return true;
}

bool USBSerialHandler::USBCallback_request(void) {
    /* Called in ISR context */
	//XPCC_LOG_DEBUG .printf("USBCallback_request\n");

    bool success = false;
    CONTROL_TRANSFER * transfer = device->getTransferPtr();

    /* Process class-specific requests */

    if (transfer->setup.bmRequestType.Type == CLASS_TYPE) {
        switch (transfer->setup.bRequest) {
            case CDC_GET_LINE_CODING:
                transfer->remaining = 7;
                transfer->ptr = (uint8_t*)cdc_line_coding;
                transfer->direction = DEVICE_TO_HOST;
                success = true;
                break;
            case CDC_SET_LINE_CODING:
                transfer->remaining = 7;
                success = true;
                terminal_connected = true;
                break;
            case CDC_SET_CONTROL_LINE_STATE:
                terminal_connected = false;
                success = true;
                break;
            default:
                break;
        }
    }

    return success;
}

bool USBSerialHandler::USBCallback_setConfiguration(uint8_t configuration) {

	if (configuration != DEFAULT_CONFIGURATION) {
        return false;
    }

    // Configure endpoints > 0
    device->addEndpoint(intIn, MAX_PACKET_SIZE_EPINT);
    device->addEndpoint(bulkIn, MAX_PACKET_SIZE_EPBULK);
    device->addEndpoint(bulkOut, MAX_PACKET_SIZE_EPBULK);

    // We activate the endpoint to be able to recceive data
    device->readStart(bulkOut, MAX_PACKET_SIZE_EPBULK);
    return true;
}
