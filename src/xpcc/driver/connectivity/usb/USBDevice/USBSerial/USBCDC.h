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

#ifndef CDC_EPBULK_OUT
#define CDC_EPBULK_OUT EPBULK_OUT
#endif
#ifndef CDC_EPBULK_IN
#define CDC_EPBULK_IN EPBULK_IN
#endif
#ifndef CDC_EPINT_IN
#define CDC_EPINT_IN EPINT_IN
#endif

#ifndef USBCDC_H
#define USBCDC_H


/* These headers are included for child class. */

//#include <xpcc/architecture.hpp>

//#include "USBEndpoints.h"
#include "../USBDevice/USBDescriptor.h"
//#include "USBDevice_Types.h"
#include "../DFU.hpp"
#include "../USBDevice/USBDevice.h"

namespace xpcc {

class USBCDC: public USBDevice {
public:

    /*
    * Constructor
    *
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your preoduct_release
    */
    USBCDC(){}



protected:
    
    /*
    * Get configuration descriptor
    *
    * @returns pointer to the configuration descriptor
    */



    
protected:
    //virtual bool USBCallback_request();
    //virtual bool USBCallback_setConfiguration(uint8_t configuration);
    //volatile bool terminal_connected;

};

}

#endif
