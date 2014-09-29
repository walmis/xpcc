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

class USBSerial: public USBCDC, public xpcc::IODevice {
public:

    /**
    *   Constructor
    *
    * @param vendor_id Your vendor_id (default: 0x1f00)
    * @param product_id Your product_id (default: 0x2012)
    * @param product_release Your preoduct_release (default: 0x0001)
    *
    */
	USBSerial(uint16_t vendor_id = 0x1f00, uint16_t product_id = 0x2012,
			uint16_t product_release = 0x0001) :
			USBCDC(vendor_id, product_id, product_release) {

		this->addInterfaceHandler(handler);

	};

    void
	write(char c){
    	handler.putc(c);
    }

    //TODO: implement flush
	void flush(){};

	/// Read a single character
	bool read(char& c) {
		return handler.getc(c);
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
