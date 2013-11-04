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
#include <xpcc/workflow.hpp>

#define CALL_HANDLERS(x) \
    	if(handlers) { \
    		auto h = handlers; \
    		while(h) { \
    			h->x; \
    			h = h->next; \
    		} \
    	}	 \

namespace xpcc {

class USBHAL : xpcc::TickerTask {
public:
    /* Configuration */
    USBHAL();
    ~USBHAL();
    void connect(void);
    void disconnect(void);
    void configureDevice(void);
    void unconfigureDevice(void);
    void setAddress(uint8_t address);
    void remoteWakeup(void);

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
    bool realiseEndpoint(uint8_t endpoint, uint32_t maxPacket, uint32_t options);
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


protected:

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
    	CALL_HANDLERS(connectStateChanged(connected));
    };
    virtual void suspendStateChanged(unsigned int suspended){
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
    //virtual EP handlers, this order must be preserved
    //since they are called directly from vtable
    virtual bool EP1_OUT_callback(){return false;};
    virtual bool EP1_IN_callback(){return false;};
    virtual bool EP2_OUT_callback(){return false;};
    virtual bool EP2_IN_callback(){return false;};
    virtual bool EP3_OUT_callback(){return false;};
    virtual bool EP3_IN_callback(){return false;};
    virtual bool EP4_OUT_callback(){return false;};
    virtual bool EP4_IN_callback(){return false;};
    
#ifndef __ARM_LPC11UXX__
    virtual bool EP5_OUT_callback(){return false;};
    virtual bool EP5_IN_callback(){return false;};
    virtual bool EP6_OUT_callback(){return false;};
    virtual bool EP6_IN_callback(){return false;};
    virtual bool EP7_OUT_callback(){return false;};
    virtual bool EP7_IN_callback(){return false;};
    virtual bool EP8_OUT_callback(){return false;};
    virtual bool EP8_IN_callback(){return false;};
    virtual bool EP9_OUT_callback(){return false;};
    virtual bool EP9_IN_callback(){return false;};
    virtual bool EP10_OUT_callback(){return false;};
    virtual bool EP10_IN_callback(){return false;};
    virtual bool EP11_OUT_callback(){return false;};
    virtual bool EP11_IN_callback(){return false;};
    virtual bool EP12_OUT_callback(){return false;};
    virtual bool EP12_IN_callback(){return false;};
    virtual bool EP13_OUT_callback(){return false;};
    virtual bool EP13_IN_callback(){return false;};
    virtual bool EP14_OUT_callback(){return false;};
    virtual bool EP14_IN_callback(){return false;};
    virtual bool EP15_OUT_callback(){return false;};
    virtual bool EP15_IN_callback(){return false;};
#endif
    
private:
    void usbisr(void);
    void handleInterrupt(int irqn) override;


};

}

