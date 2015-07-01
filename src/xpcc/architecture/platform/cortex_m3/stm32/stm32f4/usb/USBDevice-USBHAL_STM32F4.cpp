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


#include <xpcc/architecture.hpp>
#include <xpcc/driver/connectivity/usb/USBDevice/USBDevice/USBHAL.h>
#include "USBDevice-USBEndpoints_STM32F4.h"
#include "USBDevice-USBRegs_STM32.h"
#include <string.h>

using namespace xpcc;

static USBHAL* instance;

static volatile int epComplete = 0;

static uint32_t bufferEnd = 0;
static const uint32_t rxFifoSize = 512;
static volatile uint32_t rxFifoCount = 0;

static uint32_t setupBuffer[MAX_PACKET_SIZE_EP0 >> 2];

uint32_t USBHAL::endpointReadcore(uint8_t endpoint, uint8_t *buffer) {
    return 0;
}

void USBHAL::enableInterrupts() {
    NVIC_EnableIRQ(OTG_FS_IRQn);
}

void USBHAL::disableInterrupts() {
	NVIC_DisableIRQ(OTG_FS_IRQn);
}

USBHAL::USBHAL(void) {
    NVIC_DisableIRQ(OTG_FS_IRQn);

    // Enable power and clocking
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

//#if defined(TARGET_STM32F407VG) || defined(TARGET_STM32F401RE) || defined(TARGET_STM32F411RE)
//    pin_function(PA_8, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS));
//    pin_function(PA_9, STM_PIN_DATA(STM_MODE_INPUT, GPIO_PULLDOWN, GPIO_AF10_OTG_FS));
//    pin_function(PA_10, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_PULLUP, GPIO_AF10_OTG_FS));
//    pin_function(PA_11, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS));
//    pin_function(PA_12, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF10_OTG_FS));
//#else
//    pin_function(PA_8, STM_PIN_DATA(2, 10));
//    pin_function(PA_9, STM_PIN_DATA(0, 0));
//    pin_function(PA_10, STM_PIN_DATA(2, 10));
//    pin_function(PA_11, STM_PIN_DATA(2, 10));
//    pin_function(PA_12, STM_PIN_DATA(2, 10));
    stm32::PA11::setFunction(stm32::AltFunction::AF_OTG_FS);
    stm32::PA12::setFunction(stm32::AltFunction::AF_OTG_FS);

    // Set ID pin to open drain with pull-up resistor
    //pin_mode(PA_10, OpenDrain);
    //GPIOA->PUPDR &= ~(0x3 << 20);
    //GPIOA->PUPDR |= 1 << 20;

    // Set VBUS pin to open drain
    //pin_mode(PA_9, OpenDrain);
//#endif

    RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN;

    // Enable interrupts
    OTG_FS->GREGS.GAHBCFG |= (1 << 0);


    OTG_FS->GREGS.GUSBCFG |= (0xF << 10) | // Turnaround time to maximum value - too small causes packet loss
    						 (1<<30); //force device mode

    // Unmask global interrupts
    OTG_FS->GREGS.GINTMSK |= USB_OTG_GINTMSK_IEPINT |
    						USB_OTG_GINTMSK_RXFLVLM | // RX FIFO not empty
							USB_OTG_GINTMSK_USBRST |
							USB_OTG_GINTMSK_SOFM |
							USB_OTG_GINTMSK_USBSUSPM; // USB reset

    OTG_FS->DREGS.DCFG |= (0x3 << 0) | // Full speed
                          USB_OTG_DCFG_NZLSOHSK; // Non-zero-length status OUT handshake

    OTG_FS->GREGS.GCCFG |= //(1 << 19) | // Enable VBUS sensing
    					   USB_OTG_GCCFG_NOVBUSSENS | //disable vbus sensing
                           (1 << 16); // Power Up

    instance = this;
//    NVIC_SetVector(OTG_FS_IRQn, (uint32_t)&_usbisr);
//    NVIC_SetPriority(OTG_FS_IRQn, 1);
}

USBHAL::~USBHAL(void) {
	instance = 0;
}

void USBHAL::connect(void) {
    NVIC_EnableIRQ((IRQn_Type)OTG_FS_IRQn);
}

void USBHAL::disconnect(void) {
    NVIC_DisableIRQ((IRQn_Type)OTG_FS_IRQn);
}

void USBHAL::configureDevice(void) {
    // Not needed
}

void USBHAL::unconfigureDevice(void) {
    // Not needed
}

void USBHAL::setAddress(uint8_t address) {
	OTG_FS->DREGS.DCFG &= ~0x7F0;
	OTG_FS->DREGS.DCFG |= (address << 4);
    EP0write(0, 0);
}

bool USBHAL::realiseEndpoint(uint8_t endpoint, uint32_t maxPacket,
                             uint32_t flags, EPType t) {
    uint32_t epIndex = endpoint >> 1;

    uint32_t type = 0;
    switch (t) {
        case EPType::Control:
            type = 0;
            break;
        case EPType::Isochronous:
            type = 1;
            break;
        case EPType::Bulk:
            type = 2;
            break;
        case EPType::Interrupt:
            type = 3;
            break;
    }

    if(endpoint > 7)
    	return false;

    // Generic in or out EP controls
    uint32_t control = (maxPacket << 0) | // Packet size
                       (1 << 15) | // Active endpoint
                       (type << 18); // Endpoint type

    //XPCC_LOG_DEBUG .printf("realize %d type %d\n", endpoint, type);

    if (endpoint & 0x1) { // In Endpoint
        // Set up the Tx FIFO
    	uint8_t psize;
        if (endpoint == EP0IN) {
        	psize = (maxPacket >> 2);
            OTG_FS->GREGS.DIEPTXF0_HNPTXFSIZ = (psize << 16) |
                                               (bufferEnd << 0);
        }
        else {
//        	if(t == EPType::Bulk) {
//        		psize = (maxPacket >> 1); //double buffer
//        	} else {
        		psize = (maxPacket >> 2); //single buffer for interrupt transfers
        	//}
            OTG_FS->GREGS.DIEPTXF[epIndex - 1] = (psize << 16) |
                                                 (bufferEnd << 0);
        }
        bufferEnd += psize;


        // Set the In EP specific control settings
        if (endpoint != EP0IN) {
            control |= (1 << 28); // SD0PID
        }

        control |= (epIndex << 22) | // TxFIFO index
                  	USB_OTG_DIEPCTL_SNAK; // SNAK
        OTG_FS->INEP_REGS[epIndex].DIEPCTL = control;

        // Unmask the interrupt
        OTG_FS->DREGS.DAINTMSK |= (1 << epIndex);
    }
    else { // Out endpoint
        // Set the out EP specific control settings
        control |= (1 << 26); // CNAK
        OTG_FS->OUTEP_REGS[epIndex].DOEPCTL = control;

        // Unmask the interrupt
        OTG_FS->DREGS.DAINTMSK |= (1 << (epIndex + 16));
    }
    return true;
}

// read setup packet
void USBHAL::EP0setup(uint8_t *buffer) {
    memcpy(buffer, setupBuffer, MAX_PACKET_SIZE_EP0);
}

void USBHAL::EP0readStage(void) {
}

void USBHAL::EP0read(void) {
}

uint32_t USBHAL::EP0getReadResult(uint8_t *buffer) {
    uint32_t* buffer32 = (uint32_t *) buffer;
    uint32_t length = rxFifoCount;
    for (uint32_t i = 0; i < length; i += 4) {
        buffer32[i >> 2] = OTG_FS->FIFO[0][0];
    }

    rxFifoCount = 0;
    return length;
}

void USBHAL::EP0write(uint8_t *buffer, uint32_t size) {
    endpointWrite(0, buffer, size);
}

void USBHAL::EP0getWriteResult(void) {
}

void USBHAL::EP0stall(void) {
    // If we stall the out endpoint here then we have problems transferring
    // and setup requests after the (stalled) get device qualifier requests.
    // TODO: Find out if this is correct behavior, or whether we are doing
    // something else wrong
    stallEndpoint(EP0IN);
    //XPCC_LOG_DEBUG << "EP0 stall\n";
//    stallEndpoint(EP0OUT);
}

EP_STATUS USBHAL::endpointRead(uint8_t endpoint, uint32_t maximumSize) {
    uint32_t epIndex = endpoint >> 1;
    uint32_t size = (1 << 19) | // 1 packet
                    (maximumSize << 0); // Packet size
//    if (endpoint == EP0OUT) {
        size |= (1 << 29); // 1 setup packet
//    }
    OTG_FS->OUTEP_REGS[epIndex].DOEPTSIZ = size;
    OTG_FS->OUTEP_REGS[epIndex].DOEPCTL |= USB_OTG_DOEPCTL_EPENA | // Enable endpoint
    									   USB_OTG_DOEPCTL_CNAK; // Clear NAK

    epComplete &= ~(1 << endpoint);
    return EP_PENDING;
}

EP_STATUS USBHAL::endpointReadResult(uint8_t endpoint, uint8_t * buffer, uint32_t *bytesRead) {
	if (!(epComplete & (1 << endpoint))) {
		*bytesRead = 0;
        return EP_PENDING;
    }

    uint32_t length = rxFifoCount;
    //if(length > 64) {
    	//XPCC_LOG_DEBUG .printf("rd%d\n", rxFifoCount);
    //}
    for (uint32_t i = 0; i < length; i += 4) {
    	*(__packed uint32_t *)buffer = OTG_FS->FIFO[endpoint >> 1][0];
    	buffer+=4;
    }
    rxFifoCount = 0;
    *bytesRead = length;
    return EP_COMPLETED;
}

EP_STATUS USBHAL::endpointWrite(uint8_t endpoint, uint8_t *data, uint32_t size) {
	//XPCC_LOG_DEBUG .printf("wr %d\n", size);
    uint32_t epIndex = endpoint >> 1;
    OTG_FS->INEP_REGS[epIndex].DIEPTSIZ = (1 << 19) | // 1 packet
                                          (size << 0); // Size of packet
    OTG_FS->INEP_REGS[epIndex].DIEPCTL |= (1 << 31) | // Enable endpoint
                                          (1 << 26); // CNAK
    OTG_FS->DREGS.DIEPEMPMSK = (1 << epIndex);

    //uint_fast32_t deadlockPreventer = 100000;
    while ((OTG_FS->INEP_REGS[epIndex].DTXFSTS & 0XFFFF) < ((size + 3) >> 2) /*&& deadlockPreventer--*/);
    /*if(deadlockPreventer == 0) {
    	XPCC_LOG_DEBUG << "timeout\n";
    }*/

    for (uint32_t i=0; i<(size + 3) >> 2; i++, data+=4) {
        OTG_FS->FIFO[epIndex][0] = *(__packed uint32_t *)data;
    }

    epComplete &= ~(1 << endpoint);

    return EP_PENDING;
}

EP_STATUS USBHAL::endpointWriteResult(uint8_t endpoint) {
    if (epComplete & (1 << endpoint)) {
        epComplete &= ~(1 << endpoint);
        return EP_COMPLETED;
    }

    return EP_PENDING;
}

void USBHAL::stallEndpoint(uint8_t endpoint) {
    if (endpoint & 0x1) { // In EP
        OTG_FS->INEP_REGS[endpoint >> 1].DIEPCTL |= (1 << 30) | // Disable
                                                    (1 << 21); // Stall
    }
    else {  // Out EP
        OTG_FS->DREGS.DCTL |= (1 << 9); // Set global out NAK
        OTG_FS->OUTEP_REGS[endpoint >> 1].DOEPCTL |= (1 << 30) | // Disable
                                                     (1 << 21); // Stall
    }
}

void USBHAL::unstallEndpoint(uint8_t endpoint) {

}

bool USBHAL::getEndpointStallState(uint8_t endpoint) {
    return false;
}

void USBHAL::remoteWakeup(void) {
}

static void disableEP() {
	  for (int i = 1; i < NUMBER_OF_LOGICAL_ENDPOINTS; i++) {
	    /* Disable only if enabled because this sentence in the manual:
	       "The application must set this bit only if Endpoint Enable is
	        already set for this endpoint".*/
	    if ((OTG_FS->INEP_REGS[i].DIEPCTL & USB_OTG_DIEPCTL_EPENA) != 0) {
	    	OTG_FS->INEP_REGS[i].DIEPCTL = USB_OTG_DIEPCTL_EPDIS;
	      /* Wait for endpoint disable.*/
	      //while (!(OTG_FS->INEP_REGS[i].DIEPINT & USB_OTG_DIEPINT_EPDISD))
	      //  ;
	    }
	    else {
	    	OTG_FS->INEP_REGS[i].DIEPCTL = 0;
	    }
	    OTG_FS->INEP_REGS[i].DIEPTSIZ = 0;
	    OTG_FS->INEP_REGS[i].DIEPINT = 0xFFFFFFFF;
	    /* Disable only if enabled because this sentence in the manual:
	       "The application must set this bit only if Endpoint Enable is
	        already set for this endpoint".
	       Note that the attempt to disable the OUT EP0 is ignored by the
	       hardware but the code is simpler this way.*/
	    if ((OTG_FS->OUTEP_REGS[i].DOEPCTL & USB_OTG_DOEPCTL_EPENA) != 0) {
	    	OTG_FS->OUTEP_REGS[i].DOEPCTL = USB_OTG_DOEPCTL_EPDIS;
	      /* Wait for endpoint disable.*/
	      //while (!(OTG_FS->OUTEP_REGS[i].DOEPINT & USB_OTG_DOEPINT_OTEPDIS))
	      //  ;
	    }
	    else
	    	OTG_FS->OUTEP_REGS[i].DOEPCTL = 0;
	    OTG_FS->OUTEP_REGS[i].DOEPTSIZ = 0;
	    OTG_FS->OUTEP_REGS[i].DOEPINT = 0xFFFFFFFF;
	  }
	  OTG_FS->DREGS.DAINTMSK = USB_OTG_DAINTMSK_IEPM | USB_OTG_DAINTMSK_OEPM;
}

void USBHAL::_usbisr(void) {

    //GpioProfiler<stm32::PB15> p;
    if (OTG_FS->GREGS.GINTSTS & USB_OTG_GINTMSK_USBRST) { // USB Reset
        //XPCC_LOG_DEBUG << "USB Reset\n";


//        OTG_FS->GREGS.GRSTCTL = USB_OTG_GRSTCTL_CSRST;
//        while (OTG_FS->GREGS.GRSTCTL & USB_OTG_GRSTCTL_CSRST);
//        OTG_FS->GREGS.GINTSTS = USB_OTG_GINTMSK_USBRST;
//
////        // Enable interrupts
//        OTG_FS->GREGS.GAHBCFG |= (1 << 0);
//
//
//        OTG_FS->GREGS.GUSBCFG |= (0xF << 10) | // Turnaround time to maximum value - too small causes packet loss
//        						 (1<<30); //force device mode
//
//        // Unmask global interrupts
//        OTG_FS->GREGS.GINTMSK |= USB_OTG_GINTMSK_IEPINT |
//        						USB_OTG_GINTMSK_RXFLVLM | // RX FIFO not empty
//    							USB_OTG_GINTMSK_USBRST |
//    							USB_OTG_GINTMSK_SOFM; // USB reset


    	// Set SNAK bits
    	for (int i = 0; i < NUMBER_OF_LOGICAL_ENDPOINTS; i++) {
    		OTG_FS->OUTEP_REGS[i].DOEPCTL = USB_OTG_DOEPCTL_SNAK;
    		OTG_FS->INEP_REGS[i].DIEPCTL =  USB_OTG_DIEPCTL_SNAK;
    		OTG_FS->INEP_REGS[i].DIEPINT = 0xFF;
    		OTG_FS->OUTEP_REGS[i].DOEPINT = 0xFF;

//    		XPCC_LOG_DEBUG .printf("EP%d ctl in:%x out:%x\n", i, OTG_FS->INEP_REGS[i].DIEPCTL,
//    				OTG_FS->OUTEP_REGS[i].DOEPCTL);
        }

    	//disableEP();

    	OTG_FS->DREGS.DAINT = 0xFFFFFFFF;

        OTG_FS->DREGS.DIEPMSK = USB_OTG_DIEPMSK_XFRCM;

        bufferEnd = 0;
        // Set the receive FIFO size
        OTG_FS->GREGS.GRXFSIZ = rxFifoSize >> 2;
        bufferEnd += rxFifoSize >> 2;

        /// Flush RX fifo
        OTG_FS->GREGS.GRSTCTL = USB_OTG_GRSTCTL_RXFFLSH;
        while (OTG_FS->GREGS.GRSTCTL & USB_OTG_GRSTCTL_RXFFLSH);
        /* Wait for 3 PHY Clocks.*/
        for(int i = 0; i < 24; i++) {
        	asm volatile("nop");
        }
        ////

        setAddress(0);

        //flush all rx tx fifos
        OTG_FS->GREGS.GRSTCTL = USB_OTG_GRSTCTL_TXFNUM_4 | USB_OTG_GRSTCTL_TXFFLSH;

        //wait for fifo flush
        while(OTG_FS->GREGS.GRSTCTL & USB_OTG_GRSTCTL_TXFFLSH);
        for(int i = 0; i < 24; i++) {
        	asm volatile("nop");
        }
        //////////


        // Create the endpoints, and wait for setup packets on out EP0
        realiseEndpoint(EP0IN, MAX_PACKET_SIZE_EP0, 0, USBHAL::EPType::Control);
        realiseEndpoint(EP0OUT, MAX_PACKET_SIZE_EP0, 0, USBHAL::EPType::Control);
        endpointRead(EP0OUT, MAX_PACKET_SIZE_EP0);

        OTG_FS->GREGS.GINTSTS = USB_OTG_GINTMSK_USBRST;

        busReset();

        if(_suspended) {
        	suspendStateChanged(false);
        }

        return;
    }

    if (OTG_FS->GREGS.GINTSTS & USB_OTG_GINTMSK_SOFM) { // Start of frame
        SOF((OTG_FS->GREGS.GRXSTSR >> 17) & 0xF);
        OTG_FS->GREGS.GINTSTS = USB_OTG_GINTMSK_SOFM;
    }

    if (OTG_FS->GREGS.GINTSTS & USB_OTG_GINTMSK_RXFLVLM) { // RX FIFO not empty
        uint32_t status = OTG_FS->GREGS.GRXSTSP;

        uint32_t endpoint = (status & 0xF) << 1;
        uint32_t length = (status >> 4) & 0x7FF;
        uint32_t type = (status >> 17) & 0xF;

        rxFifoCount = length;

        if (type == 0x6) {
            // Setup packet
            for (uint32_t i=0; i<length; i+=4) {
                setupBuffer[i >> 2] = OTG_FS->FIFO[0][i >> 2];
            }
            rxFifoCount = 0;
        }

        if (type == 0x4) {
            // Setup complete
            EP0setupCallback();
            endpointRead(EP0OUT, MAX_PACKET_SIZE_EP0);
        }

        if (type == 0x2) {
            // Out packet
            if (endpoint == EP0OUT) {
                EP0out();
            } else {
                //XPCC_LOG_DEBUG .printf("ep%d L%d T%d\n", endpoint, length, type);

                epComplete |= (1 << endpoint);

				if(EP_handler(endpoint)) {
					epComplete &= ~(1 << endpoint);
				}
            }
        }

        for (uint32_t i=0; i<rxFifoCount; i+=4) {
            (void) OTG_FS->FIFO[0][0];
        }
        OTG_FS->GREGS.GINTSTS = USB_OTG_GINTMSK_RXFLVLM;
    }

    if (OTG_FS->GREGS.GINTSTS & (USB_OTG_GINTMSK_USBSUSPM)) {

    	bool sus = OTG_FS->DREGS.DSTS & 1;
    	//XPCC_LOG_DEBUG .printf("sus %d\n", sus);
    	if(_suspended != sus) {
    		suspendStateChanged(sus);
    	}
    	OTG_FS->GREGS.GINTSTS = USB_OTG_GINTMSK_USBSUSPM;
    }

    if (OTG_FS->GREGS.GINTSTS & USB_OTG_GINTMSK_IEPINT) { // In endpoint interrupt

    	// Loop through the in endpoints
        for (uint32_t i=0; i<4; i++) {
            if (OTG_FS->DREGS.DAINT & (1 << i)) { // Interrupt is on endpoint
                // If the transfer is complete
                if (OTG_FS->INEP_REGS[i].DIEPINT & USB_OTG_DIEPINT_XFRC) { // Tx Complete

					uint32_t id = (1 + (i << 1));
					epComplete |= (1 << id);
					OTG_FS->INEP_REGS[i].DIEPINT = USB_OTG_DIEPINT_XFRC;

                	if(i > 0) {
						if(EP_handler(id)) {
							epComplete &= ~(1 << id);
						}
                	}

                }

                if (OTG_FS->INEP_REGS[i].DIEPINT & USB_OTG_DIEPINT_TXFE) {// Tx FIFO empty

                    // If the Tx FIFO is empty on EP0 we need to send a further
                    // packet, so call EP0in()
                    if (i == 0) {
                        EP0in();
                    }

                    // Clear the interrupt
                    OTG_FS->INEP_REGS[i].DIEPINT = USB_OTG_DIEPINT_TXFE;
                    // Stop firing Tx empty interrupts
                    // Will get turned on again if another write is called
                    //XPCC_LOG_DEBUG .printf("TXFE %d\n", OTG_FS->INEP_REGS[i].DTXFSTS & 0XFFFF);
                    OTG_FS->DREGS.DIEPEMPMSK &= ~(1 << i);

                }

            }
        }
        OTG_FS->GREGS.GINTSTS = USB_OTG_GINTMSK_IEPINT;
    }


}

extern "C"
void OTG_FS_IRQHandler() {
	IRQWrapper w;
	if(instance)
		instance->_usbisr();
}
