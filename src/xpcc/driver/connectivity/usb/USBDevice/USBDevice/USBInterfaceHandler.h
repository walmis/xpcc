#pragma once

//#include "USBDevice.h"
#include <stdint.h>

namespace xpcc {
class USBDevice;

class USBInterfaceHandler {
public:
	USBInterfaceHandler() {
		device = 0;
		next = 0;
	}

protected:
    /*
    * Called by USBDevice layer on bus reset. Warning: Called in ISR context
    *
    * May be used to reset state
    */
    virtual void USBCallback_busReset(void) {};

    /*
    * Called by USBDevice on Endpoint0 request. Warning: Called in ISR context
    * This is used to handle extensions to standard requests
    * and class specific requests
    *
    * @returns true if class handles this request
    */
    virtual bool USBCallback_request() { return false; };

    /*
    * Called by USBDevice on Endpoint0 request completion
    * if the 'notify' flag has been set to true. Warning: Called in ISR context
    *
    * In this case it is used to indicate that a HID report has
    * been received from the host on endpoint 0
    *
    * @param buf buffer received on endpoint 0
    * @param length length of this buffer
    */
    virtual void USBCallback_requestCompleted(uint8_t * buf, uint32_t length) {};

    /*
    * Called by USBDevice layer. Set configuration of the device.
    * For instance, you can add all endpoints that you need on this function.
    *
    * @param configuration Number of the configuration
    */
    virtual bool USBCallback_setConfiguration(uint8_t configuration) { return false; };

    /*
     * Called by USBDevice layer. Set interface/alternate of the device.
     *
     * @param interface Number of the interface to be configured
     * @param alternate Number of the alternate to be configured
     * @returns true if class handles this request
     */
    virtual bool USBCallback_setInterface(uint16_t interface, uint8_t alternate) { return false; };

    /*
    * Get device descriptor. Warning: this method has to store the length of the report descriptor in reportLength.
    *
    * @returns pointer to the device descriptor
    * */

    virtual void EP0setupCallback(void){};
    virtual void EP0out(void){};
    virtual void EP0in(void){};
    virtual void connectStateChanged(unsigned int connected){};
    virtual void suspendStateChanged(unsigned int suspended){};

    virtual void SOF(int frameNumber){};

    virtual bool EP_handler(uint8_t ep) {return false;};


    friend class USBHAL;
    friend class USBDevice;


	USBDevice* device;
	USBInterfaceHandler* next;


};

}
