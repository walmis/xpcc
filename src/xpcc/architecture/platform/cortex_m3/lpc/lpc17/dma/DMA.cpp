/*
    Copyright (c) 2010 Andy Kirkham
 
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
 
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
 
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include "DMA.h"

namespace xpcc {

// Create a "hook" for our ISR to make callbacks. Set by init()
DMA *DMA::moddma_p = 0;

void
DMA::Enable(CHANNELS ChannelNumber)
{
    LPC_GPDMACH_TypeDef *pChannel = (LPC_GPDMACH_TypeDef *)Channel_p( ChannelNumber );
    pChannel->DMACCConfig |= _E;
}

bool
DMA::Enabled(CHANNELS ChannelNumber)
{
    LPC_GPDMACH_TypeDef *pChannel = (LPC_GPDMACH_TypeDef *)Channel_p( ChannelNumber );    
    return (bool)(pChannel->DMACCConfig & _E);
}

void
DMA::Disable(CHANNELS ChannelNumber)
{
    LPC_GPDMACH_TypeDef *pChannel = (LPC_GPDMACH_TypeDef *)Channel_p( ChannelNumber );
    pChannel->DMACCConfig &= ~(_E);
}

bool
DMA::isActive(CHANNELS ChannelNumber)
{
    LPC_GPDMACH_TypeDef *pChannel = (LPC_GPDMACH_TypeDef *)Channel_p( ChannelNumber );
    return (bool)( pChannel->DMACCConfig & CxConfig_A() ) ;
}

void 
DMA::haltChannel(CHANNELS ChannelNumber)
{
    LPC_GPDMACH_TypeDef *pChannel = (LPC_GPDMACH_TypeDef *)Channel_p( ChannelNumber );
    pChannel->DMACCConfig |= CxConfig_H();
}

uint32_t 
DMA::getControl(CHANNELS ChannelNumber)
{
    LPC_GPDMACH_TypeDef *pChannel = (LPC_GPDMACH_TypeDef *)Channel_p( ChannelNumber );
    return pChannel->DMACCControl;
}

uint32_t oldDMAHandler = 0;
typedef void (*MODDMA_FN)(void);

}; // namespace AjK ends

extern "C"
void DMA_IRQHandler(void) {
    uint32_t channel_mask;
        
    if (xpcc::DMA::moddma_p == 0) {
        if (xpcc::oldDMAHandler) {
            ((xpcc::MODDMA_FN)xpcc::oldDMAHandler)();
            return;
        }
        else {
            XPCC_LOG_ERROR .printf("Interrupt without instance");
        }
    }
    
    for (int channel_number = 0; channel_number < 8; channel_number++) {
        channel_mask = (1UL << channel_number);
        if (LPC_GPDMA->DMACIntStat & channel_mask) {
            if (LPC_GPDMA->DMACIntTCStat & channel_mask) {
                if (xpcc::DMA::moddma_p->setups[channel_number] != 0) {
                    xpcc::DMA::moddma_p->setIrqProcessingChannel((xpcc::DMA::CHANNELS)channel_number);
                    xpcc::DMA::moddma_p->setIrqType(xpcc::DMA::TcIrq);
                    xpcc::DMA::moddma_p->setups[channel_number]->isrIntTCStat->call();
                    xpcc::DMA::moddma_p->isrIntTCStat.call();
                    // The user callback should clear the IRQ. But if they forget
                    // then the Mbed will lockup. So, check to see if the IRQ has
                    // been dismissed, if not, we will dismiss it here.
                    if (LPC_GPDMA->DMACIntTCStat & channel_mask) {
                        LPC_GPDMA->DMACIntTCClear = channel_mask;
                    }
                    // If the user has left the channel enabled, disable it.
                    // Note, we don't check Active here as it may block inside
                    // an ISR, we just shut it down immediately. If the user
                    // must wait for completion they should implement their
                    // own ISR. But only disable if the LLI linked list register
                    // is null otherwise we can crap out a series of transfers.
                    if (xpcc::DMA::moddma_p->Enabled( (xpcc::DMA::CHANNELS)channel_number )) {
                        if (xpcc::DMA::moddma_p->lli( (xpcc::DMA::CHANNELS)channel_number ) == 0 ) {
                            xpcc::DMA::moddma_p->Disable( (xpcc::DMA::CHANNELS)channel_number );
                        }
                    }
                }            
            }
            
            if (LPC_GPDMA->DMACIntErrStat & channel_mask) {
                if (xpcc::DMA::moddma_p->setups[channel_number] != 0) {
                	xpcc::DMA::moddma_p->setIrqProcessingChannel((xpcc::DMA::CHANNELS)channel_number);
                	xpcc::DMA::moddma_p->setIrqType(xpcc::DMA::ErrIrq);
                	xpcc::DMA::moddma_p->setups[channel_number]->isrIntErrStat->call();
                	xpcc::DMA::moddma_p->isrIntErrStat.call();
                    // The user callback should clear the IRQ. But if they forget
                    // then the Mbed will lockup. So, check to see if the IRQ has
                    // been dismissed, if not, we will dismiss it here.
                    if (LPC_GPDMA->DMACIntErrStat & channel_mask) {
                        LPC_GPDMA->DMACIntErrClr = channel_mask;
                    }
                    // If the user has left the channel enabled, disable it.
                    // Not, we don't check Active here as it may block inside
                    // an ISR, we just shut it down immediately. If the user
                    // must wait for completion they should implement their
                    // own ISR. But only disable if the LLI linked list register
                    // is null otherwise we can crap out a series of transfers.
                    if (xpcc::DMA::moddma_p->Enabled( (xpcc::DMA::CHANNELS)channel_number )) {
                        if (xpcc::DMA::moddma_p->lli( (xpcc::DMA::CHANNELS)channel_number ) == 0 ) {
                        	xpcc::DMA::moddma_p->Disable( (xpcc::DMA::CHANNELS)channel_number );
                        }
                    }
                }            
            }
        }
    }
    
    /* IRQ should be handled by now, check to make sure. */
    if (LPC_GPDMA->DMACIntStat) {
        ((xpcc::MODDMA_FN)xpcc::oldDMAHandler)();
        LPC_GPDMA->DMACIntTCClear = (uint32_t)0xFF; /* If not, clear anyway! */
    }
    if (LPC_GPDMA->DMACIntErrStat) {
        ((xpcc::MODDMA_FN)xpcc::oldDMAHandler)();
        LPC_GPDMA->DMACIntErrClr = (uint32_t)0xFF; /* If not, clear anyway! */
    }
}



