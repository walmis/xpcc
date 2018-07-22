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

#ifndef USBSERIAL_H
#define USBSERIAL_H

#include "USBCDC.h"
#include "USBSerialHandler.hpp"

#include <xpcc/io.hpp>
#include <xpcc/architecture/driver.hpp>

namespace xpcc {

#include "USBCDC.h"
  
/**
* \brief		USB Serial Class

* \ingroup		usb
*/  
  
/**
* USBSerial example
*
* @code
* #include "mbed.h"
* #include "USBSerial.h"
*
* //Virtual serial port over USB
* USBSerial serial;
*
* int main(void) {
*
*    while(1)
*    {
*        serial.printf("I am a virtual serial port\n");
*        wait(1);
*    }
* }
* @endcode
*/

class USBSerial final: public USBCDC, public xpcc::IODevice {
public:
	USBSerial() {
		addInterfaceHandler(handler);
	};

    size_t
	write(char c){
    	handler.putch(c);
    	return 1;
    }

    //TODO: implement flush
	void flush(){};

	/// Read a single character
	int16_t read() override {
		return handler.getch();
	}

	void setLatency(uint8_t ms) {
		handler.setLatency(ms);
	}


    /**
    * Check the number of bytes available.
    *
    * @returns the number of bytes available
    */
    int16_t rxAvailable() {
    	return handler.rxAvailable();
    }

    int16_t txAvailable() {
    	return handler.txAvailable();
    }
    
    USBSerialHandler handler;

};

}

#endif
