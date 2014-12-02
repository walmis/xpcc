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
    
    @file          MODDMA.h 
    @purpose       Adds DMA controller and multiple transfer configurations
    @version       see ChangeLog.c
    @date          Nov 2010
    @author        Andy Kirkham    
*/

#ifndef MODDMA_H
#define MODDMA_H

/** @ingroup lpc17 */
/** @defgroup API DMA */

#include <lpc17xx/cmsis/LPC17xx.h>
#include <xpcc/processing.hpp>

//#include "iomacros.h"

extern "C" void DMA_IRQHandler();

namespace xpcc {
namespace lpc17 {

//forward reference
class DMAConfig;
class DMA;

//! Channel definitions.
enum DMAChannel {
      Channel_0 = 0     /*!< Channel 0 */
    , Channel_1         /*!< Channel 1 */
    , Channel_2         /*!< Channel 2 */
    , Channel_3         /*!< Channel 3 */
    , Channel_4         /*!< Channel 4 */
    , Channel_5         /*!< Channel 5 */
    , Channel_6         /*!< Channel 6 */
    , Channel_7         /*!< Channel 7 */
};

//! DMA Connection number definitions
enum DMAConnection {
      SSP0_Tx       = 0UL   /*!< SSP0 Tx */
    , SSP0_Rx       = 1UL   /*!< SSP0 Rx */
    , SSP1_Tx       = 2UL   /*!< SSP1 Tx */
    , SSP1_Rx       = 3UL   /*!< SSP1 Rx */
    , ADC           = 4UL   /*!< ADC */
    , I2S_Channel_0 = 5UL   /*!< I2S channel 0 */
    , I2S_Channel_1 = 6UL   /*!< I2S channel 1 */
    , DAC           = 7UL   /*!< DAC */
    , UART0_Tx      = 8UL   /*!< UART0 Tx */
    , UART0_Rx      = 9UL   /*!< UART0 Rx */
    , UART1_Tx      = 10UL  /*!< UART1 Tx */
    , UART1_Rx      = 11UL  /*!< UART1 Rx */
    , UART2_Tx      = 12UL  /*!< UART2 Tx */
    , UART2_Rx      = 13UL  /*!< UART2 Rx */
    , UART3_Tx      = 14UL  /*!< UART3 Tx */
    , UART3_Rx      = 15UL  /*!< UART3 Rx */
    , MAT0_0        = 16UL  /*!< MAT0.0 */
    , MAT0_1        = 17UL  /*!< MAT0.1 */
    , MAT1_0        = 18UL  /*!< MAT1.0 */
    , MAT1_1        = 19UL  /*!< MAT1.1 */
    , MAT2_0        = 20UL  /**< MAT2.0 */
    , MAT2_1        = 21UL  /*!< MAT2.1 */
    , MAT3_0        = 22UL  /*!< MAT3.0 */
    , MAT3_1        = 23UL  /*!< MAT3.1 */
};

//! GPDMA Transfer type definitions
enum  DMATransferType {
      m2m = 0     /*!< Memory to memory - DMA control */
    , m2p = 1     /*!< Memory to peripheral - DMA control */
    , p2m = 2     /*!< Peripheral to memory - DMA control */
    , p2p = 3     /*!< Src peripheral to dest peripheral - DMA control */
    , g2m = 4     /*!< Psuedo special case for reading "peripheral GPIO" that's memory mapped. */
    , m2g = 5     /*!< Psuedo Special case for writing "peripheral GPIO" that's memory mapped. */
};

//! Burst size in Source and Destination definitions */
enum DMABurstSize {
      _1    = 0   /*!< Burst size = 1 */
    , _4    = 1   /*!< Burst size = 4 */
    , _8    = 2   /*!< Burst size = 8 */
    , _16   = 3   /*!< Burst size = 16 */
    , _32   = 4   /*!< Burst size = 32 */
    , _64   = 5   /*!< Burst size = 64 */
    , _128  = 6   /*!< Burst size = 128 */
    , _256  = 7   /*!< Burst size = 256 */
};

//! Width in Src transfer width and Dest transfer width definitions */
enum DMATransferWidth {
      byte     = 0    /*!< Width = 1 byte */
    , halfword = 1    /*!< Width = 2 bytes */
    , word     = 2    /*!< Width = 4 bytes */
};


/**
 * @brief The MODDMA configuration system (linked list items)
 * @author Andy Kirkham
 * @see http://mbed.org/cookbook/MODDMA_Config
 * @see MODDMA
 * @see MODDMA_Config
 * @ingroup API
 */
class DMALLI {
public:
	DMALLI() {
		SrcAddr = 0;
		DstAddr = 0;
		NextLLI = 0;
		Control = 0;
	}
	DMALLI(DMAConfig* cfg);

	void reset(DMAConfig* cfg);

	/// Set transfer size for LLI transfer
	DMALLI* transferSize(uint16_t n);

    class DMALLI *srcAddr(void* n) { SrcAddr = (uint32_t)n; return this; }
    class DMALLI *dstAddr(void* n) { DstAddr = (uint32_t)n; return this; }
    class DMALLI *nextLLI(DMALLI* n) { NextLLI = (uint32_t)n; return this; }
    class DMALLI *control(uint32_t n) { Control = n; return this; }
    uint32_t srcAddr(void) { return SrcAddr; }
    uint32_t dstAddr(void) { return DstAddr; }
    DMALLI* nextLLI(void) { return (DMALLI*)NextLLI; }
    uint32_t control(void) { return Control; }

    uint32_t SrcAddr;    //!< Source Address
    uint32_t DstAddr;    //!< Destination address
    uint32_t NextLLI;    //!< Next LLI address, otherwise set to '0'
    uint32_t Control;    //!< GPDMA Control of this LLI
};

/**
 * @brief The DMA configuration system
 * @author Andy Kirkham
 * @see http://mbed.org/cookbook/MODDMA_Config
 * @ingroup API
 *
 * <b>DMAConfig</b> defines a configuration that can be passed to the DMA controller
 * instance to perform a GPDMA data transfer.
 */
class  DMAConfig {
protected:

    // *****************************************
    // From GPDMA by NXP MCU SW Application Team
    // *****************************************
    
    uint8_t ChannelNum;        //!< DMA channel number, should be in range from 0 to 7.
    uint16_t TransferSize;      //!< Length/Size of transfer

    uint8_t srcTransferWidth : 4;
    uint8_t dstTransferWidth : 4;

    uint8_t sbsize : 4;
    uint8_t dbsize : 4;

    void* SrcMemAddr;        //!< Physical Src Addr, used in case TransferType is chosen as MODDMA::GPDMA_TRANSFERTYPE::m2m or MODDMA::GPDMA_TRANSFERTYPE::m2p
    void* DstMemAddr;        //!< Physical Destination Address, used in case TransferType is chosen as MODDMA::GPDMA_TRANSFERTYPE::m2m or MODDMA::GPDMA_TRANSFERTYPE::p2m
    uint8_t TransferType;      //!< Transfer Type
    uint8_t SrcConn;           //!< Peripheral Source Connection type, used in case TransferType is chosen as
    uint8_t DstConn;           //!< Peripheral Destination Connection type, used in case TransferType is chosen as
    DMALLI* LLI;            //!< Linker List Item structure data address if there's no Linker List, set as '0'
    uint8_t DMACSync;          //!< DMACSync if required.
    uint8_t flags;

public: 

    enum Flags {
    	DEFAULTS = 0,
    	FORCE_SI_ON = 1<<0, ///< Force source address increment on
		FORCE_SI_OFF = 1<<1, ///< Force source address increment off
		FORCE_DI_ON = 1<<2, ///< Force destination address increment on
		FORCE_DI_OFF = 1<<3 ///< Force destination address increment off
	};

    DMAConfig() {
    	LLI = 0;
    	reset();
    }

    void reset() {
    	ChannelNum    = 0xFF;
        TransferSize  = 0;
        srcTransferWidth = 0xF;
        dstTransferWidth = 0xF;
        sbsize = 0xF;
        dbsize = 0xF;
        SrcMemAddr    = 0;
        DstMemAddr    = 0;
        TransferType  = 0;
        SrcConn       = 0;
        DstConn       = 0;
        LLI           = 0;
        DMACSync      = 0;
        flags         = 0;
    }
        
    ALWAYS_INLINE DMAConfig * channelNum(DMAChannel n)    { ChannelNum = n & 0x7;  return this; }
    ALWAYS_INLINE DMAConfig * transferSize(uint32_t n)    { TransferSize = n;      return this; }
    ALWAYS_INLINE DMAConfig * transferWidth(DMATransferWidth n) {
    	dstTransferWidth = srcTransferWidth = (uint8_t)n;
    	return this;
    }
    ALWAYS_INLINE DMAConfig * SWidth(DMATransferWidth n) { srcTransferWidth = (uint8_t)n;     return this; }
    ALWAYS_INLINE DMAConfig * DWidth(DMATransferWidth n) { dstTransferWidth = (uint8_t)n;     return this; }
    ALWAYS_INLINE DMAConfig * srcMemAddr(void* n)    { SrcMemAddr = n;        return this; }
    ALWAYS_INLINE DMAConfig * dstMemAddr(void* n)    { DstMemAddr = n;        return this; }
    ALWAYS_INLINE DMAConfig * transferType(DMATransferType n)  { TransferType = n;      return this; }
    ALWAYS_INLINE DMAConfig * srcConn(DMAConnection n)  { SrcConn = n;           return this; }
    ALWAYS_INLINE DMAConfig * dstConn(DMAConnection n)  { DstConn = n;           return this; }
    ALWAYS_INLINE DMAConfig * dmaLLI(DMALLI* n)        { LLI = n;               return this; }
    ALWAYS_INLINE DMAConfig * dmacSync(DMAConnection n)      { DMACSync = n;          return this; }
    ALWAYS_INLINE DMAConfig * transferFlags(Flags n)    { flags = (uint8_t)n;    return this; }
    /// Source burst size
    ALWAYS_INLINE DMAConfig * SBurstSize(DMABurstSize n)  { sbsize = (uint8_t)n; return this; }
    /// Destination burst size
    ALWAYS_INLINE DMAConfig * DBurstSize(DMABurstSize n)  { dbsize = (uint8_t)n; return this; }
    
    uint32_t channelNum(void)    { return ChannelNum;    }
    uint32_t transferSize(void)  { return TransferSize;  }
    uint32_t SWidth(void)      { return srcTransferWidth; }
    uint32_t DWidth(void)      { return dstTransferWidth; }
    void* srcMemAddr(void)    { return SrcMemAddr;    }
    void* dstMemAddr(void)    { return DstMemAddr;    }
    uint32_t transferType(void)  { return TransferType;  }
    uint32_t srcConn(void)       { return SrcConn;       }
    uint32_t dstConn(void)       { return DstConn;       }
    DMALLI* dmaLLI(void)        { return LLI;        }
    uint32_t dmacSync(void)      { return DMACSync; }
    uint8_t transferFlags()      { return flags; }
    
    uint8_t SBurstSize()    { return sbsize; }
    uint8_t DBurstSize()    { return dbsize; }

    inline void setup();
    inline void enable();
    inline void disable();
    inline bool enabled();
    inline void halt();

    /**
     * Attach a callback to the TC IRQ configuration.
     *
     * @param fptr A function pointer to call
     * @return this
     */
    class DMAConfig * attach_tc(xpcc::function<void()> fptr) {
        isrIntTCStat.attach(fptr);
        return this;
    }
    
    /**
     * Attach a callback to the ERR IRQ configuration.
     *
     * @param fptr A function pointer to call
     * @return this
     */
    class DMAConfig * attach_err(xpcc::function<void()> fptr) {
        isrIntErrStat.attach(fptr);
        return this;
    }
    
    /**
     * Attach a callback to the TC IRQ configuration.
     *
     * @param tptr A template pointer to the calling object
     * @param mptr A method pointer within the object to call.
     * @return this
     */
    template<typename T>
    class DMAConfig * attach_tc(T* tptr, void (T::*mptr)(void)) {  
        if((mptr != NULL) && (tptr != NULL)) {
            isrIntTCStat.attach(tptr, mptr);
        }
        return this;
    }
    
    /**
     * Attach a callback to the ERR IRQ configuration.
     *
     * @param tptr A template pointer to the calling object
     * @param mptr A method pointer within the object to call.
     * @return this
     */
    template<typename T>
    class DMAConfig * attach_err(T* tptr, void (T::*mptr)(void)) {  
        if((mptr != NULL) && (tptr != NULL)) {
            isrIntErrStat.attach(tptr, mptr);
        }
        return this;
    }

    FunctionPointer isrIntTCStat;
    FunctionPointer isrIntErrStat;
};


 /**
 * @brief MODDMA GPDMA Controller
 * @author Andy Kirkham
 * @see http://mbed.org/cookbook/MODDMA
 * @see example1.cpp
 * @ingroup API
 *
 * <b>MODDMA</b> defines a GPDMA controller and multiple DMA configurations that allow for DMA
 * transfers from memory to memory, memory to peripheral or peripheral to memory.
 *
 * At the heart of the library is the MODDMA class that defines a single instance controller that
 * manages all the GPDMA hardware registers and interrupts. The controller can accept multiple
 * configurations that define the channel transfers. Each configuration specifies the source and 
 * destination information and other associated parts to maintain the transfer process.
 *
 * Standard example:
 * @code
 * #include "mbed.h"
 * #include "MODDMA.h"
 *
 * DigitalOut led1(LED1);
 * Serial pc(USBTX, USBRX); // tx, rx
 * MODDMA dma;
 *
 * int main() {
 *
 *     // Create a string buffer to send directly to a Uart/Serial
 *     char s[] = "***DMA*** ABCDEFGHIJKLMNOPQRSTUVWXYZ ***DMA***";
 *
 *     // Create a transfer configuarion
 *     MODDMA_Config *config = new MODDMA_Config;
 *
 *     // Provide a "minimal" setup for demo purposes.
 *     config
 *      ->channelNum    ( MODDMA::Channel_0 )   // The DMA channel to use.
 *      ->srcMemAddr    ( (uint32_t) &s )       // A pointer to the buffer to send.
 *      ->transferSize  ( sizeof(s) )           // The size of that buffer.
 *      ->transferType  ( MODDMA::m2p )         // Source is memory, destination is peripheral
 *      ->dstConn       ( MODDMA::UART0_Tx )    // Specifically, peripheral is Uart0 TX (USBTX, USBRX)
 *    ; // config end.
 *
 *    // Pass the configuration to the MODDMA controller.
 *    dma.Setup( config );
 *
 *    // Enable the channel and begin transfer.
 *    dma.Enable( config->channelNum() );
 *
 *    while(1) {
 *         led1 = !led1;
 *         wait(0.25);
 *     }
 * }
 * @endcode
 */
class DMA
{
public:
    
    //! Interrupt callback types.
    enum IrqType_t {
          TcIrq = 0     /*!< Terminal Count interrupt */
        , ErrIrq        /*!< Error interrupt */
    };
    
    //! Return status codes.
    enum Status {
          Ok            = 0     /*!< Ok, suceeded */
        , Error         = -1    /*!< General error */
        , ErrChInUse    = -2    /*!< Specific error, channel in use */
    };
    

    
    //! DMA Request Select Mode definitions. */
    enum GPDMA_REQSEL {
          uart  = 0UL   /*!< UART TX/RX is selected */
        , timer = 1UL   /*!< Timer match is selected */
    };

    //! GPDMA Control register bits.
    enum Config {
          _E = 1        /*!< DMA Controller enable */
        , _M = 2        /*!< AHB Master endianness configuration */
    };    

    //! GPDMA Channel config register bits.
    enum CConfig {
          _CE  = (1UL << 0)     /*!< Channel enable */
        , _IE  = (1UL << 14)    /*!< Interrupt error mask */
        , _ITC = (1UL << 15)    /*!< Terminal count interrupt mask */
        , _LOCK   = (1UL << 16)    /*!< Lock */
        , _A   = (1UL << 17)    /*!< Active */
        , _H   = (1UL << 18)    /*!< Halt */
    };
    

    static DMA* instance() {
    	if(!moddma_p) {
    		moddma_p = new DMA;
    	}
    	return moddma_p;
    }
    

    /**
     * Used to setup and enable the DMA controller.
     *
     * @see Setup
     * @see Enable
     * @param c A pointer to an instance of MODDMA_Config to setup.
     */
    uint32_t Prepare(DMAConfig *c) {
        uint32_t u = Setup(c);
        if (u) Enable(c);
        return u;
    }
    
    /**
     * Used to setup the DMA controller to prepare for a data transfer.
     *
     * @param c A pointer to an instance of MODDMA_Config to setup.
     */
    uint32_t Setup(DMAConfig *c);
    
    /**
     * Enable and begin data transfer.
     *
     * @param ChannelNumber Type DMAChannel, the channel number to enable
     */
    inline void Enable(DMAChannel ChannelNumber) {
        LPC_GPDMACH_TypeDef *pChannel = (LPC_GPDMACH_TypeDef *)Channel_p( ChannelNumber );
        pChannel->DMACCConfig |= _E;
    }
    
    /**
     * Enable and begin data transfer (overloaded function)
     *
     * @param ChannelNumber Type uin32_t, the channel number to enable
     */
    inline void Enable(uint32_t ChannelNumber) { Enable((DMAChannel)(ChannelNumber & 0x7)); }
    
    /**
     * Enable and begin data transfer (overloaded function)
     *
     * @param config A pointer to teh configuration
     */
    inline void Enable(DMAConfig *config) { Enable( config->channelNum() ); }
        
    
    /**
     * Disable a channel and end data transfer.
     *
     * @ingroup API
     * @param ChannelNumber Type DMAChannel, the channel number to enable
     */
    inline void Disable(DMAChannel ChannelNumber) {
        LPC_GPDMACH_TypeDef *pChannel = (LPC_GPDMACH_TypeDef *)Channel_p( ChannelNumber );
        pChannel->DMACCConfig &= ~(_E);
    }
    
    /**
     * Disable a channel and end data transfer (overloaded function)
     *
     * @ingroup API
     * @param ChannelNumber Type uin32_t, the channel number to disable
     */
    inline void Disable(uint32_t ChannelNumber) { Disable((DMAChannel)(ChannelNumber & 0x7)); }
    
    /**
     * Is the specified channel enabled?
     *
     * @param ChannelNumber Type DMAChannel, the channel number to test
     * @return bool true if enabled, false otherwise.
     */
    inline bool Enabled(DMAChannel ChannelNumber) {
        LPC_GPDMACH_TypeDef *pChannel = (LPC_GPDMACH_TypeDef *)Channel_p( ChannelNumber );
        return (bool)(pChannel->DMACCConfig & _E);
    }
    
    /**
     * Returns LPC_DMA->DMACControl value for a specific channel configuration
     *
     * @param Pointer to channel configuration
     * @return DMACControl value
     */
    static uint32_t buildControlValue(DMAConfig* cfg);

    static void getSrcDestAddresses(DMAConfig* cfg, uint32_t& srcAddr, uint32_t& destAddr);

    /**
     * Is the specified channel enabled? (overloaded function)
     *
      * @param ChannelNumber Type uin32_t, the channel number to test
     * @return bool true if enabled, false otherwise.
     */
    inline bool Enabled(uint32_t ChannelNumber) { return Enabled((DMAChannel)(ChannelNumber & 0x7)); }
    
    static ALWAYS_INLINE uint32_t IntStat(uint32_t n)            { return (1UL << n) & 0xFF; }
    static ALWAYS_INLINE uint32_t IntTCStat_Ch(uint32_t n)       { return (1UL << n) & 0xFF; }
    static ALWAYS_INLINE uint32_t IntTCClear_Ch(uint32_t n)      { return (1UL << n) & 0xFF; }
    static ALWAYS_INLINE uint32_t IntErrStat_Ch(uint32_t n)      { return (1UL << n) & 0xFF; }
    static ALWAYS_INLINE uint32_t IntErrClr_Ch(uint32_t n)       { return (1UL << n) & 0xFF; }
    static ALWAYS_INLINE uint32_t RawIntErrStat_Ch(uint32_t n)   { return (1UL << n) & 0xFF; }
    static ALWAYS_INLINE uint32_t EnbldChns_Ch(uint32_t n)       { return (1UL << n) & 0xFF; }
    static ALWAYS_INLINE uint32_t SoftBReq_Src(uint32_t n)       { return (1UL << n) & 0xFFFF; }
    static ALWAYS_INLINE uint32_t SoftSReq_Src(uint32_t n)       { return (1UL << n) & 0xFFFF; }
    static ALWAYS_INLINE uint32_t SoftLBReq_Src(uint32_t n)      { return (1UL << n) & 0xFFFF; }
    static ALWAYS_INLINE uint32_t SoftLSReq_Src(uint32_t n)      { return (1UL << n) & 0xFFFF; }
    static ALWAYS_INLINE uint32_t Sync_Src(uint32_t n)           { return (1UL << n) & 0xFFFF; }
    static ALWAYS_INLINE uint32_t ReqSel_Input(uint32_t n)       { return (1UL << (n - 8)) & 0xFF; }
    

    static ALWAYS_INLINE uint32_t CxControl_TransferSize(uint32_t n)     { return (n & 0xFFF) << 0; }
    static ALWAYS_INLINE uint32_t CxControl_SBSize(uint32_t n)           { return (n & 0x7) << 12; }
    static ALWAYS_INLINE uint32_t CxControl_DBSize(uint32_t n)           { return (n & 0x7) << 15; }
    static ALWAYS_INLINE uint32_t CxControl_SWidth(uint32_t n)           { return (n & 0x7) << 18; }
    static ALWAYS_INLINE uint32_t CxControl_DWidth(uint32_t n)           { return (n & 0x7) << 21; }
    static ALWAYS_INLINE uint32_t CxControl_SI()                         { return (1UL << 26); }
    static ALWAYS_INLINE uint32_t CxControl_DI()                         { return (1UL << 27); }
    static ALWAYS_INLINE uint32_t CxControl_Prot1()                      { return (1UL << 28); }
    static ALWAYS_INLINE uint32_t CxControl_Prot2()                      { return (1UL << 29); }
    static ALWAYS_INLINE uint32_t CxControl_Prot3()                      { return (1UL << 30); }
    static ALWAYS_INLINE uint32_t CxControl_I()                          { return (1UL << 31); }
    static ALWAYS_INLINE uint32_t CxControl_E()                          { return (1UL << 0); }
    static ALWAYS_INLINE uint32_t CxConfig_SrcPeripheral(uint32_t n)     { return (n & 0x1F) << 1; }
    static ALWAYS_INLINE uint32_t CxConfig_DestPeripheral(uint32_t n)    { return (n & 0x1F) << 6; }
    static ALWAYS_INLINE uint32_t CxConfig_TransferType(uint32_t n)      { return (n & 0x7) << 11; }
    static ALWAYS_INLINE uint32_t CxConfig_IE()                          { return (1UL << 14); }
    static ALWAYS_INLINE uint32_t CxConfig_ITC()                         { return (1UL << 15); }
    static ALWAYS_INLINE uint32_t CxConfig_L()                           { return (1UL << 16); }
    static ALWAYS_INLINE uint32_t CxConfig_A()                           { return (1UL << 17); }
    static ALWAYS_INLINE uint32_t CxConfig_H()                           { return (1UL << 18); }
    
    /**
     * A store for up to 8 (8 DMAChannel) of configurations.
     * @see MODDMA_Config
     */
    DMAConfig *setups[8];
    
    /**
     * Get a pointer to the current configuration the ISR is servicing.
     *
     * @return MODDMA_Config * A pointer to the setup the ISR is currently servicing.
     */
    DMAConfig *getConfig(void) { return setups[IrqProcessingChannel]; }
    
    
    /**
     * Gets which channel the ISR is currently servicing.
     *
     * @return DMAChannel The current channel the ISR is servicing.
     */
    DMAChannel irqProcessingChannel(void) { return IrqProcessingChannel; }
    
    /**
     * Sets which type of IRQ the ISR is making a callback for.
     *
     * *** USED INTERNALLY. DO NOT CALL FROM USER PROGRAMS ***
     *
     * Must be public so the extern "C" ISR can use it.
     */ 
    void setIrqType(IrqType_t n) { IrqType = n; }
    
    /**
     * Get which type of IRQ the ISR is calling you about,
     * terminal count or error.
     */
    IrqType_t irqType(void) { return IrqType; }
    
    /**
     * Clear the interrupt after handling.
     *
     * @param DMAChannel The channel the IQR occured on.
     */
    void clearTcIrq(DMAChannel n) { LPC_GPDMA->DMACIntTCClear = (uint32_t)(1UL << n); }
    
    /**
     * Clear the interrupt the ISR is currently handing..
     */
    void clearTcIrq(void) { clearTcIrq( IrqProcessingChannel ); }
    
    /**
     * Clear the error interrupt after handling.
     *
     * @ingroup API
     * @param DMAChannel The channel the IQR occured on.
     */
    void clearErrIrq(DMAChannel n) { LPC_GPDMA->DMACIntTCClear = (uint32_t)(1UL << n); }
    
    /**
     * Clear the error interrupt the ISR is currently handing.
     * @ingroup API
     */
    void clearErrIrq(void) { clearErrIrq( IrqProcessingChannel ); }
   
    /**
     * Is the supplied channel currently active?
     *
     * @ingroup API
     * @param DMAChannel The channel to inquire about.
     * @return bool true if active, false otherwise.
     */     
    bool isActive(DMAChannel ChannelNumber) {
        LPC_GPDMACH_TypeDef *pChannel = (LPC_GPDMACH_TypeDef *)Channel_p( ChannelNumber );
        return (bool)( pChannel->DMACCConfig & CxConfig_A() ) ;
    }

    bool isActive(DMAConfig* config) {
    	return isActive((DMAChannel)config->channelNum());
    }
    
    /**
     * Halt the supplied channel. 
     *
     * @ingroup API
     * @param DMAChannel The channel to halt.
     */
    inline void haltChannel(DMAChannel ChannelNumber) {
        LPC_GPDMACH_TypeDef *pChannel = (LPC_GPDMACH_TypeDef *)Channel_p( ChannelNumber );
        pChannel->DMACCConfig |= CxConfig_H();
    }
    
    /**
     * get a DMAChannel control register.
     *
     * @ingroup API
     * @param DMAChannel The channel to get the control register for.
     */
    inline uint32_t getControl(DMAChannel ChannelNumber) {
        LPC_GPDMACH_TypeDef *pChannel = (LPC_GPDMACH_TypeDef *)Channel_p( ChannelNumber );
        return pChannel->DMACCControl;
    }
    
    uint32_t getControl(DMAConfig *cfg) {
    	return getControl((DMAChannel)cfg->channelNum());
    }

    /**
     * Get number of bytes remaining to transfer
     *
     * @param DMAChannel The channel to get the control register for.
     */
    uint32_t dataRemaining(DMAChannel ChannelNumber) {
    	return getControl((DMAChannel)ChannelNumber) & 0xFFF;
    }

    /**
     * Get number of bytes remaining to transfer
     *
     * @param DMAChannel The channel to get the control register for.
     */
    uint32_t dataRemaining(DMAConfig *cfg) {
    	return getControl((DMAChannel)cfg->channelNum()) & 0xFFF;
    }

    /**
     * Wait for channel transfer to complete and then halt.
     *
     * @ingroup API
     * @param DMAChannel The channel to wait for then halt.
     */
    void haltAndWaitChannelComplete(DMAChannel n) { haltChannel(n); while (isActive(n)); }
    
    /**
     * Attach a callback to the TC IRQ controller.
     *
     * @param fptr A function pointer to call
     * @return this
     */
    void attach_tc(void (*fptr)(void)) {  
        isrIntTCStat.attach(fptr);         
    }
    
    /**
     * Attach a callback to the TC IRQ controller.
     *
     * @ingroup API
     * @param tptr A template pointer to the calling object
     * @param mptr A method pointer within the object to call.
     * @return this
     */
    template<typename T>
    void attach_tc(T* tptr, void (T::*mptr)(void)) {  
        if((mptr != NULL) && (tptr != NULL)) {
            isrIntTCStat.attach(tptr, mptr);         
        }        
    }
       
    /**
     * The MODDMA controllers terminal count interrupt callback.
     */
    FunctionPointer isrIntTCStat;                        
    
    /**
     * Attach a callback to the ERR IRQ controller.
     *
     * @param fptr A function pointer to call
     * @return this
     */
    void attach_err(void (*fptr)(void)) {  
        isrIntErrStat.attach(fptr);                 
    }
    
    /**
     * Attach a callback to the ERR IRQ controller.
     *
     * @param tptr A template pointer to the calling object
     * @param mptr A method pointer within the object to call.
     * @return this
     */
    template<typename T>
    void attach_err(T* tptr, void (T::*mptr)(void)) {  
        if((mptr != NULL) && (tptr != NULL)) {
            isrIntErrStat.attach(tptr, mptr);         
        }
    }
    
    /**
     * Get the Linked List index regsiter for the requested channel.
     *
     * @param channelNum The channel number.
     * @return uint32_t The value of the DMACCLLI register
     */
    uint32_t lli(DMAChannel ChannelNumber, DMALLI *set = 0) {
        LPC_GPDMACH_TypeDef *pChannel = (LPC_GPDMACH_TypeDef *)Channel_p( ChannelNumber & 0x7 );
        if (set) pChannel->DMACCLLI = (uint32_t)set;
        return pChannel->DMACCLLI; 
    }
    
    /**
     * The MODDMA controllers error interrupt callback.
     */
   FunctionPointer isrIntErrStat;
    
   static uint32_t Channel_p(int channel) {
		const uint32_t lut[] = { (uint32_t) LPC_GPDMACH0,
				(uint32_t) LPC_GPDMACH1, (uint32_t) LPC_GPDMACH2,
				(uint32_t) LPC_GPDMACH3, (uint32_t) LPC_GPDMACH4,
				(uint32_t) LPC_GPDMACH5, (uint32_t) LPC_GPDMACH6,
				(uint32_t) LPC_GPDMACH7 };
		return lut[channel & 0xFF];
	}
    
protected:

    /**
     * Set which channel the ISR is currently servicing.
     *
     * *** USED INTERNALLY. DO NOT CALL FROM USER PROGRAMS ***
     *
     * Must be public so the extern "C" ISR can use it.
     */
    inline void setIrqProcessingChannel(DMAChannel n) { IrqProcessingChannel = n; }

    /**
     * The MODDMA constructor is used to initialise the DMA controller object.
     */
    DMA() { init(true); }

    /**
     * The MODDMA destructor.
     */
    ~DMA() {}

    /**
     * Used to setup the DMA controller to prepare for a data transfer.
     *
     * @ingroup API
     * @param isConstructorCalling Set true when called from teh constructor
     * @param
     */
    void init(bool isConstructorCalling, int DMAChannel = 0xFF, int Tc = 0xFF, int Err = 0xFF);
   
    // Data LUTs.
    static uint32_t LUTPerAddr(int n) {
        const uint32_t lut[] = {
              (uint32_t)&LPC_SSP0->DR         // SSP0 Tx
            , (uint32_t)&LPC_SSP0->DR         // SSP0 Rx
            , (uint32_t)&LPC_SSP1->DR         // SSP1 Tx
            , (uint32_t)&LPC_SSP1->DR         // SSP1 Rx
            , (uint32_t)&LPC_ADC->ADGDR       // ADC
            , (uint32_t)&LPC_I2S->I2STXFIFO   // I2S Tx
            , (uint32_t)&LPC_I2S->I2SRXFIFO   // I2S Rx
            , (uint32_t)&LPC_DAC->DACR        // DAC
            , (uint32_t)&LPC_UART0->THR       // UART0 Tx
            , (uint32_t)&LPC_UART0->RBR       // UART0 Rx
            , (uint32_t)&LPC_UART1->THR       // UART1 Tx
            , (uint32_t)&LPC_UART1->RBR       // UART1 Rx
            , (uint32_t)&LPC_UART2->THR       // UART2 Tx
            , (uint32_t)&LPC_UART2->RBR       // UART2 Rx
            , (uint32_t)&LPC_UART3->THR       // UART3 Tx
            , (uint32_t)&LPC_UART3->RBR       // UART3 Rx
            , (uint32_t)&LPC_TIM0->MR0        // MAT0.0
            , (uint32_t)&LPC_TIM0->MR1        // MAT0.1
            , (uint32_t)&LPC_TIM1->MR0        // MAT1.0
            , (uint32_t)&LPC_TIM1->MR1        // MAT1.1
            , (uint32_t)&LPC_TIM2->MR0        // MAT2.0
            , (uint32_t)&LPC_TIM2->MR1        // MAT2.1
            , (uint32_t)&LPC_TIM3->MR0        // MAT3.0
            , (uint32_t)&LPC_TIM3->MR1        // MAT3.1
        };
        return lut[n & 0xFF];
    }
    static uint8_t  LUTPerBurst(int n) {
        const uint8_t lut[] = {
              (uint8_t)_4       // SSP0 Tx
            , (uint8_t)_4       // SSP0 Rx
            , (uint8_t)_4       // SSP1 Tx
            , (uint8_t)_4       // SSP1 Rx
            , (uint8_t)_1       // ADC
            , (uint8_t)_32      // I2S channel 0
            , (uint8_t)_32      // I2S channel 1
            , (uint8_t)_1       // DAC
            , (uint8_t)_1       // UART0 Tx
            , (uint8_t)_1       // UART0 Rx
            , (uint8_t)_1       // UART1 Tx
            , (uint8_t)_1       // UART1 Rx
            , (uint8_t)_1       // UART2 Tx
            , (uint8_t)_1       // UART2 Rx
            , (uint8_t)_1       // UART3 Tx
            , (uint8_t)_1       // UART3 Rx
            , (uint8_t)_1       // MAT0.0
            , (uint8_t)_1       // MAT0.1
            , (uint8_t)_1       // MAT1.0
            , (uint8_t)_1       // MAT1.1
            , (uint8_t)_1       // MAT2.0
            , (uint8_t)_1       // MAT2.1
            , (uint8_t)_1       // MAT3.0
            , (uint8_t)_1       // MAT3.1
        };
        return lut[n & 0xFFF];
    }
    static uint8_t  LUTPerWid(int n) {
        const uint8_t lut[] = {
              (uint8_t)byte      // SSP0 Tx
            , (uint8_t)byte      // SSP0 Rx
            , (uint8_t)byte      // SSP1 Tx
            , (uint8_t)byte      // SSP1 Rx
            , (uint8_t)word      // ADC
            , (uint8_t)word      // I2S channel 0
            , (uint8_t)word      // I2S channel 1
            , (uint8_t)word      // DAC
            , (uint8_t)byte      // UART0 Tx
            , (uint8_t)byte      // UART0 Rx
            , (uint8_t)byte      // UART1 Tx
            , (uint8_t)byte      // UART1 Rx
            , (uint8_t)byte      // UART2 Tx
            , (uint8_t)byte      // UART2 Rx
            , (uint8_t)byte      // UART3 Tx
            , (uint8_t)byte      // UART3 Rx
            , (uint8_t)word      // MAT0.0
            , (uint8_t)word      // MAT0.1
            , (uint8_t)word      // MAT1.0
            , (uint8_t)word      // MAT1.1
            , (uint8_t)word      // MAT2.0
            , (uint8_t)word      // MAT2.1
            , (uint8_t)word      // MAT3.0
            , (uint8_t)word      // MAT3.1
        };
        return lut[n & 0xFFF];
    }
    //uint32_t Channel_p(int channel);
    
    DMAChannel IrqProcessingChannel;
    
    IrqType_t IrqType;


    friend void ::DMA_IRQHandler();

    static DMA* moddma_p;
};

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

void DMAConfig::halt() {
	DMA::instance()->haltChannel((DMAChannel)ChannelNum);
}

};
}

#endif
