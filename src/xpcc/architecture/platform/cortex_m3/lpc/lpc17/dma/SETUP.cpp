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
namespace lpc17 {

void
DMA::getSrcDestAddresses(DMAConfig* config, uint32_t& srcAddr, uint32_t& destAddr) {
    switch (config->transferType()) {

        // Memory to memory
        case m2m:
            // Assign physical source and destination address
        	srcAddr  = (uint32_t)config->srcMemAddr();
        	destAddr = (uint32_t)config->dstMemAddr();

            break;

        // Memory to peripheral
        case m2p:
        	// Assign physical source
        	srcAddr = (uint32_t)config->srcMemAddr();
            // Assign peripheral destination address
        	destAddr = (uint32_t)LUTPerAddr(config->dstConn());

            break;

        // Peripheral to memory
        case p2m:
            // Assign peripheral source address
        	srcAddr = (uint32_t)LUTPerAddr(config->srcConn());
            // Assign memory destination address
        	destAddr = (uint32_t)config->dstMemAddr();

            break;

        // Peripheral to peripheral
        case p2p:
            // Assign peripheral source address
        	srcAddr = (uint32_t)LUTPerAddr(config->srcConn());
            // Assign peripheral destination address
        	destAddr = (uint32_t)LUTPerAddr(config->dstConn());

            break;

        // GPIO to memory
        case g2m:
            // Assign GPIO source address
        	srcAddr = (uint32_t)config->srcMemAddr();
            // Assign memory destination address
        	destAddr = (uint32_t)config->dstMemAddr();

            break;

        // Memory to GPIO
        case m2g:
            // Assign physical source
        	srcAddr = (uint32_t)config->srcMemAddr();
            // Assign peripheral destination address
        	destAddr = (uint32_t)config->dstMemAddr();
            break;

    }
}


uint32_t
DMA::buildControlValue(DMAConfig* config) {
	uint32_t DMACCControl = 0;

    // Set value to Channel Control Registers 
    switch (config->transferType()) {
    
        // Memory to memory
        case m2m:
            DMACCControl
                = CxControl_TransferSize(config->transferSize()) 
                | CxControl_SBSize(config->SBurstSize() == 0xF ? _32 : config->SBurstSize())
                | CxControl_DBSize(config->DBurstSize() == 0xF ? _32: config->DBurstSize())
                | CxControl_SWidth(config->SWidth())
                | CxControl_DWidth(config->DWidth())
                | CxControl_SI() 
                | CxControl_DI() 
                | CxControl_I();
            break;
        
        // Memory to peripheral
        case m2p:

            DMACCControl
                = CxControl_TransferSize((uint32_t)config->transferSize()) 
                | CxControl_SBSize(config->SBurstSize() == 0xF ? (uint32_t)LUTPerBurst(config->dstConn()) : config->SBurstSize())
                | CxControl_DBSize((uint32_t)LUTPerBurst(config->dstConn())) 
                | CxControl_SWidth(config->SWidth() == 0xF ? LUTPerWid(config->dstConn()) : config->SWidth())
                | CxControl_DWidth((uint32_t)LUTPerWid(config->dstConn())) 
                | CxControl_SI() 
                | CxControl_I();
            break;
            
        // Peripheral to memory
        case p2m:

            DMACCControl
                = CxControl_TransferSize((uint32_t)config->transferSize()) 
                | CxControl_SBSize((uint32_t)LUTPerBurst(config->srcConn())) 
                | CxControl_DBSize(config->DBurstSize() == 0xF ? LUTPerBurst(config->srcConn()): config->DBurstSize())
                | CxControl_SWidth((uint32_t)LUTPerWid(config->srcConn())) 
                | CxControl_DWidth(config->DWidth() == 0xF ? LUTPerWid(config->srcConn()) : config->DWidth())
                | CxControl_DI() 
                | CxControl_I();
            break;
            
        // Peripheral to peripheral
        case p2p:

            DMACCControl
                = CxControl_TransferSize((uint32_t)config->transferSize()) 
                | CxControl_SBSize((uint32_t)LUTPerBurst(config->srcConn())) 
                | CxControl_DBSize((uint32_t)LUTPerBurst(config->dstConn())) 
                | CxControl_SWidth((uint32_t)LUTPerWid(config->srcConn())) 
                | CxControl_DWidth((uint32_t)LUTPerWid(config->dstConn())) 
                | CxControl_I();
            break;
            
        // GPIO to memory
        case g2m:

           DMACCControl
                = CxControl_TransferSize((uint32_t)config->transferSize()) 
                | CxControl_SBSize(config->SBurstSize() == 0xF ? (uint32_t)LUTPerBurst(config->srcConn()) : config->SBurstSize())
                | CxControl_DBSize(config->DBurstSize() == 0xF ? (uint32_t)LUTPerBurst(config->srcConn()): config->DBurstSize())
                | CxControl_SWidth(config->SWidth() == 0xF ? LUTPerWid(config->srcConn()) : config->SWidth())
                | CxControl_DWidth(config->DWidth() == 0xF ? LUTPerWid(config->srcConn()) : config->DWidth())
                | CxControl_DI() 
                | CxControl_I();
            break;
            
        // Memory to GPIO
        case m2g:

            DMACCControl
                = CxControl_TransferSize((uint32_t)config->transferSize()) 
                | CxControl_SBSize(config->SBurstSize() == 0xF ? (uint32_t)LUTPerBurst(config->dstConn()) : config->SBurstSize())
                | CxControl_DBSize(config->DBurstSize() == 0xF ? (uint32_t)LUTPerBurst(config->dstConn()): config->DBurstSize())
                | CxControl_SWidth(config->SWidth() == 0xF ? LUTPerWid(config->dstConn()) : config->SWidth())
                | CxControl_DWidth(config->DWidth() == 0xF ? LUTPerWid(config->dstConn()) : config->DWidth())
                | CxControl_SI() 
                | CxControl_I();
            break;
            
        // Do not support any more transfer type, return ERROR
        default:
            return 0;
    }

    if(config->transferFlags()) {
		if(config->transferFlags() & DMAConfig::FORCE_DI_OFF) {
			DMACCControl &= ~CxControl_DI();
		} else
		if(config->transferFlags() & DMAConfig::FORCE_DI_ON) {
			DMACCControl |= CxControl_DI();
		}

		if(config->transferFlags() & DMAConfig::FORCE_SI_OFF) {
			DMACCControl &= ~CxControl_SI();
		} else
		if(config->transferFlags() & DMAConfig::FORCE_SI_ON) {
			DMACCControl |= CxControl_SI();
		}
    }

    return DMACCControl;
}

uint32_t
DMA::Setup(DMAConfig *config)
{
   uint8_t channel = config->channelNum();

	LPC_GPDMACH_TypeDef *pChannel = (LPC_GPDMACH_TypeDef *)Channel_p( channel );

    setups[channel & 0x7] = config;

    // Reset the Interrupt status
    LPC_GPDMA->DMACIntTCClear = IntTCClear_Ch( channel );
    LPC_GPDMA->DMACIntErrClr  = IntErrClr_Ch ( channel );

    // Clear DMA configure
    pChannel->DMACCControl = 0x00;
    pChannel->DMACCConfig  = 0x00;

    // Assign Linker List Item value
    pChannel->DMACCLLI = (uint32_t)config->dmaLLI();

    pChannel->DMACCControl = buildControlValue(config);

    // Assign physical source and destination address
    getSrcDestAddresses(config, (uint32_t&)pChannel->DMACCSrcAddr, (uint32_t&)pChannel->DMACCDestAddr);

     // Re-Configure DMA Request Select for source peripheral 
    if (config->srcConn() > 15) {
        LPC_SC->DMAREQSEL |= (1 << (config->srcConn() - 16));
    } 
    else {
        LPC_SC->DMAREQSEL &= ~(1 << (config->srcConn() - 8));
    }

    // Re-Configure DMA Request Select for destination peripheral
    if (config->dstConn() > 15) {
        LPC_SC->DMAREQSEL |= (1 << (config->dstConn() - 16));
    } 
    else {
        LPC_SC->DMAREQSEL &= ~(1 << (config->dstConn() - 8));
    }

    // Enable DMA channels, little endian 
    LPC_GPDMA->DMACConfig = _E;
    while (!(LPC_GPDMA->DMACConfig & _E));

    // Calculate absolute value for Connection number
    uint32_t tmp1 = config->srcConn(); tmp1 = ((tmp1 > 15) ? (tmp1 - 8) : tmp1);
    uint32_t tmp2 = config->dstConn(); tmp2 = ((tmp2 > 15) ? (tmp2 - 8) : tmp2);

    if (config->dmacSync()) {
        uint32_t tmp3 = config->dmacSync(); tmp3 = ((tmp3 > 15) ? (tmp3 - 8) : tmp3);
        LPC_GPDMA->DMACSync |= Sync_Src( tmp3 );
    }
    
    uint32_t tfer_type = (uint32_t)config->transferType();
    if(tfer_type == m2g) {
    	tfer_type = m2p;
    } else if(tfer_type == g2m) {
    	tfer_type = p2m;
    }

    // Configure DMA Channel, enable Error Counter and Terminate counter
    pChannel->DMACCConfig 
        = CxConfig_IE() 
        | CxConfig_ITC() 
        | CxConfig_TransferType(tfer_type) 
        | CxConfig_SrcPeripheral(tmp1) 
        | CxConfig_DestPeripheral(tmp2);

    return pChannel->DMACCControl;
}

DMALLI* DMAConfig::addLLI(DMALLI* lli) {
	if(!lli)
		return 0;

	if(LLI == 0) {
		LLI = lli;
		DMA::instance()->lli((DMAChannel)ChannelNum, lli);
	} else {
		DMALLI* ptr = (DMALLI*) LLI;
		while(ptr) {
			if(ptr->nextLLI() == 0) {
				ptr->nextLLI(lli);
				break;
			}
			ptr = ptr->nextLLI();
		}

	}
	return lli;
}

DMALLI::DMALLI(DMAConfig* cfg) {
	reset(cfg);
}

void DMALLI::reset(DMAConfig* cfg) {
	NextLLI = 0;

	Control = DMA::buildControlValue(cfg);
	DMA::getSrcDestAddresses(cfg, this->SrcAddr, this->DstAddr);
}

DMALLI* DMALLI::transferSize(uint16_t n) {
	Control &= ~0xFFF;
	Control |= 	DMA::CxControl_TransferSize(n);

	return this;
}

void DMAConfig::setup() {
	DMA::instance()->Setup(this);
}

void DMAConfig::enable() {
	DMA::instance()->Enable(this);
}

void DMAConfig::disable() {
	DMA::instance()->Disable(ChannelNum);
}

bool DMAConfig::enabled() {
	return DMA::instance()->Enabled(ChannelNum);
}

}
}; // namespace AjK ends

