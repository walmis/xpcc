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

#pragma once

#include "USBInterfaceHandler.h"
#include "USBEndpoints.h"

#define CALL_HANDLERS(x) \
    	if(handlers) { \
    		auto h = handlers; \
    		while(h) { \
    			h->x; \
    			h = h->next; \
    		} \
    	}	 \

namespace xpcc {

class USBHAL {
public:

	enum class EPType {
		Control,
		Isochronous,
		Bulk,
		Interrupt
	};

    /* Configuration */
    USBHAL();
    ~USBHAL();
    void connect(void);
    void disconnect(void);
    void configureDevice(void);
    void unconfigureDevice(void);
    void setAddress(uint8_t address);
    void remoteWakeup(void);

    void disableInterrupts();
    void enableInterrupts();
    bool suspended() { return _suspended; }
    bool connected() { return _connected; }

    /* Endpoint 0 */
    void EP0setup(uint8_t *buffer);
    void EP0read(void);
    void EP0readStage(void);
    uint32_t EP0getReadResult(uint8_t *buffer);
    void EP0write(uint8_t *buffer, uint32_t size);
    void EP0getWriteResult(void);
    void EP0stall(void);

    /* Other endpoints */
    EP_STATUS endpointRead(uint8_t endpoint, uint32_t maximumSize);
    EP_STATUS endpointReadResult(uint8_t endpoint, uint8_t *data, uint32_t *bytesRead);
    EP_STATUS endpointWrite(uint8_t endpoint, uint8_t *data, uint32_t size);
    EP_STATUS endpointWriteResult(uint8_t endpoint);
    void stallEndpoint(uint8_t endpoint);
    void unstallEndpoint(uint8_t endpoint);
    bool realiseEndpoint(uint8_t endpoint, uint32_t maxPacket, uint32_t options, EPType type);
    bool getEndpointStallState(unsigned char endpoint);
    uint32_t endpointReadcore(uint8_t endpoint, uint8_t *buffer);


    void addInterfaceHandler(USBInterfaceHandler& handler) {
    	handler.device = reinterpret_cast<USBDevice*>(this);

    	if(handlers == 0) {
    		handlers = &handler;
    	} else {
    		auto h = handlers;
    		while(h) {
    			if(!h->next) {
    				h->next = &handler;
    				break;
    			}
    			h = h->next;
    		}

    	}
    }

    void _usbisr(void);

protected:
    bool _suspended;
    bool _connected;
    USBInterfaceHandler* handlers;

    virtual void busReset(void){};
    virtual void EP0setupCallback(void){
    	CALL_HANDLERS(EP0setupCallback());
    };
    virtual void EP0out(void){
    	CALL_HANDLERS(EP0out());
    };
    virtual void EP0in(void){
    	CALL_HANDLERS(EP0in());
    };
    virtual void connectStateChanged(unsigned int connected){
    	_connected = connected;
    	CALL_HANDLERS(connectStateChanged(connected));
    };
    virtual void suspendStateChanged(unsigned int suspended){
    	_suspended = suspended;
    	CALL_HANDLERS(suspendStateChanged(suspended));
    };
    virtual void SOF(int frameNumber){
    	CALL_HANDLERS(SOF(frameNumber));
    };
            
    virtual bool EP_handler(uint8_t ep) {
    	if(handlers) {
    		auto h = handlers;
    		while(h) {
    			if(h->EP_handler(ep)) {
    				return true;
    			}
    			h = h->next;
    		}
    	}
    	return false;
    }

};

}

