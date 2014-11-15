#ifndef _SPI_HPP
#define _SPI_HPP

#include "../dma/DMA.h"

#include <stdint.h>
#include <xpcc/architecture.hpp>

#include <xpcc/architecture/peripheral.hpp>
#include "spi_registers.h"

namespace xpcc {
namespace lpc17 {


/* --------------------- BIT DEFINITIONS -------------------------------------- */
/*********************************************************************//**
 * Macro defines for CR0 register
 **********************************************************************/
/** SSP data size select, must be 4 bits to 16 bits */
#define SSP_CR0_DSS(n)   		((uint32_t)((n-1)&0xF))
/** SSP control 0 Motorola SPI mode */
#define SSP_CR0_FRF_SPI  		((uint32_t)(0<<4))
/** SSP control 0 TI synchronous serial mode */
#define SSP_CR0_FRF_TI   		((uint32_t)(1<<4))
/** SSP control 0 National Micro-wire mode */
#define SSP_CR0_FRF_MICROWIRE  	((uint32_t)(2<<4))
/** SPI clock polarity bit (used in SPI mode only), (1) = maintains the
 bus clock high between frames, (0) = low */
#define SSP_CR0_CPOL_HI		((uint32_t)(1<<6))
/** SPI clock out phase bit (used in SPI mode only), (1) = captures data
 on the second clock transition of the frame, (0) = first */
#define SSP_CR0_CPHA_SECOND	((uint32_t)(1<<7))
/** SSP serial clock rate value load macro, divider rate is
 PERIPH_CLK / (cpsr * (SCR + 1)) */
#define SSP_CR0_SCR(n)   	((uint32_t)((n&0xFF)<<8))
/** SSP CR0 bit mask */
#define SSP_CR0_BITMASK		((uint32_t)(0xFFFF))

#define SSP_CPSR_BITMASK	((uint32_t)(0xFF))

#define SSP_FIFO_SIZE	(8)

#define SPIx (reinterpret_cast<LPC_SSP_TypeDef*>(Spi_ptr))

template<int Spi_ptr>
class SpiMaster: public xpcc::SpiSimpleMaster {
public:
	enum class TransferSize {
		BIT_04 = 0x03,
		BIT_05 = 0x04,
		BIT_06 = 0x05,
		BIT_07 = 0x06,
		BIT_08 = 0x07,
		BIT_09 = 0x08,
		BIT_10 = 0x09,
		BIT_11 = 0x0a,
		BIT_12 = 0x0b,
		BIT_13 = 0x0c,
		BIT_14 = 0x0d,
		BIT_15 = 0x0d,
		BIT_16 = 0x0f,
	};

	enum class FrameFormat {
		SPI = 0x00, TI = 0x01, MICROWIRE = 0x02
	};

	enum class Mode {
		MODE_0 = 0,					///< SCK normal,   sample on rising  edge
		MODE_1 = SPI_CR0_CPHA,		///< SCK normal,   sample on falling edge
		MODE_2 = SPI_CR0_CPOL,		///< SCK inverted, sample on falling edge
		MODE_3 = SPI_CR0_CPOL | SPI_CR0_CPHA,///< SCK inverted, sample on rising  edge
	};

public:
	/**
	 * @brief	Initialize SPI module
	 */
	ALWAYS_INLINE static void
	initialize(Mode mode = Mode::MODE_0, uint32_t targetFrequency =
			1000000, TransferSize transferSize = TransferSize::BIT_08,
			FrameFormat frameFormat = FrameFormat::SPI) {

		if (SPIx == LPC_SSP0) {
			CLKPwr::setClkPower(CLKPwr::PType::PCSSP0, true);
			CLKPwr::setClkDiv(CLKPwr::ClkType::SSP0, CLKPwr::ClkDiv::DIV_1);
		} else if (SPIx == LPC_SSP1) {
			CLKPwr::setClkPower(CLKPwr::PType::PCSSP1, true);
			CLKPwr::setClkDiv(CLKPwr::ClkType::SSP1, CLKPwr::ClkDiv::DIV_1);
		}

		// Control register 0
		SPIx->CR0 = (static_cast<uint16_t>(mode))
				| ((static_cast<uint16_t>(frameFormat)) << 4)
				| ((static_cast<uint16_t>(transferSize)) << 0);

		while((SPIx->SR & SPI_SRn_RNE)) {
			uint16_t Dummy = SPIx->DR; /* clear the RxFIFO */
			(void) Dummy; // unused
		}

		SPIx->CR1 = SPI_CR1_SSE;

		frequency(targetFrequency);

	}

	static void frequency(uint32_t frequency = 1000000) {
		uint32_t prescale, cr0_div, cmp_clk, ssp_clk;

		/* The SSP clock is derived from the (main system oscillator / 2),
		 so compute the best divider from that clock */

		if (SPIx == LPC_SSP0)
			ssp_clk = CLKPwr::getPCLK(CLKPwr::ClkType::SSP0);
		else
			ssp_clk = CLKPwr::getPCLK(CLKPwr::ClkType::SSP1);

		/* Find closest divider to get at or under the target frequency.
		 Use smallest prescale possible and rely on the divider to get
		 the closest target frequency */
		cr0_div = 0;
		cmp_clk = 0xFFFFFFFF;
		prescale = 2;
		while (cmp_clk > frequency) {
			cmp_clk = ssp_clk / ((cr0_div + 1) * prescale);
			if (cmp_clk > frequency) {
				cr0_div++;
				if (cr0_div > 0xFF) {
					cr0_div = 0;
					prescale += 2;
				}
			}
		}

		/* Write computed prescaler and divider back to register */
		SPIx->CR0 &= (~SSP_CR0_SCR(0xFF)) & SSP_CR0_BITMASK;
		SPIx->CR0 |= (SSP_CR0_SCR(cr0_div)) & SSP_CR0_BITMASK;
		SPIx->CPSR = prescale & SSP_CPSR_BITMASK;

	}

	/**
	 * \brief	Write a single byte with the SPI.
	 *
	 * The data is put into the FIFO but it blocks until the data
	 * was sent and data from the device is available.
	 */
	static uint16_t write(uint16_t data) {

		while (!(SPIx->SR & SPI_SRn_TFE));
		/* Put data into FIFO */
		SPIx->DR = data;

		/* Wait until data is received */
		while (!(SPIx->SR & SPI_SRn_RNE))
			;

		return SPIx->DR;
	}

	static uint8_t burstWrite(const uint8_t* buffer, uint8_t len) {
		if(len > SSP_FIFO_SIZE)
			len = SSP_FIFO_SIZE;

		while (!txFifoEmpty());

		for(int i = 0; i < len; i++) {
			SPIx->DR = *buffer++;
		}

		return len;
	}

	static uint8_t burstRead(uint8_t* buffer, uint8_t len) {
		if(len > SSP_FIFO_SIZE)
			len = SSP_FIFO_SIZE;

		while(isBusy());

		for(int i = 0; i < len; i++) {
			*buffer++ = SPIx->DR;
		}

		return len;
	}

	static inline void flushRx() {
		while(!rxFifoEmpty()) {
			uint16_t Dummy = SPIx->DR; /* clear the RxFIFO */
			(void) Dummy; // unused
		}
	}

	static inline bool rxFifoEmpty() {
		return !(SPIx->SR & SPI_SRn_RNE);
	}

	static inline bool txFifoEmpty() {
		return (SPIx->SR & SPI_SRn_TFE);
	}
	static inline bool txFifoFull() {
		return !(SPIx->SR & SPI_SRn_TNF);
	}

	static inline bool isBusy() {
		return (SPIx->SR & SPI_SRn_BSY);
	}

	static bool transfer(uint8_t * tx, uint8_t * rx, std::size_t length) {

		//XPCC_LOG_DEBUG .printf("DMA start %x %x %d\n", tx, rx, length);
		if(prepareTransfer(tx, rx, length)) {
			startTransfer(rx);
			return true;
		}
		//XPCC_LOG_DEBUG .printf("... failed\n");
		return false;
	}

	static void
	stopTransfer() {
		DMA* dma = DMA::instance();
		if(txChannelCfg) {
			//dma->haltAndWaitChannelComplete((DMAChannel)txChannelCfg->channelNum());
			dma->Disable(txChannelCfg->channelNum());

		}
		if(rxChannelCfg) {
			//dma->haltAndWaitChannelComplete((DMAChannel)rxChannelCfg->channelNum());
			dma->Disable(rxChannelCfg->channelNum());
		}
		//call isFinished() to clean up transfer
		//isFinished();
		running = false;
	}

	static void startTransfer(bool rx = false) {
		DMA* dma = DMA::instance();

		xpcc::atomic::Lock lock;
		dma->Enable(txChannelCfg);
		/* Wait until at least one byte has arrived into the RX FIFO
			   and then start-up the Channel1 DMA to begin transferring them. */
		//while((SPIx->SR & (1UL << 2)) == 0);
		//start rx transfer
		dma->Enable(rxChannelCfg);
		SPIx->DMACR = 3; // enable Tx Rx DMA
		running = true;

	}

    static bool
    prepareTransfer(uint8_t * tx, uint8_t * rx, std::size_t length) {

    	if(isTransferBusy()) {
    		return false;
    	}

    	flushRx();

    	DMA* dma = DMA::instance();

    	static const uint32_t dummy = 0xFFFFFFFF;

		DMAConnection conn = SSP0_Tx;
		if(SPIx ==LPC_SSP1)
			conn = SSP1_Tx;

		if(!txChannelCfg) {
			txChannelCfg = new (std::nothrow) DMAConfig;
		} else {
			txChannelCfg->reset();
		}

		txChannelCfg->channelNum(Channel_1)
				->dstConn(conn)
				->srcMemAddr(tx ? tx : (uint8_t*)&dummy)
				->transferSize(length)
				->transferType(m2p);

		if(!tx) {
			//force source address increment off if we send dummy bytes
			txChannelCfg->transferFlags(DMAConfig::FORCE_SI_OFF);
		}

		dma->Setup(txChannelCfg);

		conn = SSP0_Rx;
		if(SPIx == LPC_SSP1)
			conn = SSP1_Rx;

		if(!rxChannelCfg) {
			rxChannelCfg = new (std::nothrow) DMAConfig;
			if(!rxChannelCfg) {
				return 0;
			}
		} else {
			rxChannelCfg->reset();
		}

		static uint32_t _dummy_rx;

		rxChannelCfg->channelNum(Channel_0)
				->dstMemAddr((rx != 0) ? rx : (uint8_t*)&_dummy_rx)
				->srcConn(conn)
				->transferSize(length)
				->transferType(p2m)
				->attach_tc(onDMAComplete);

		if(!rx) {
			rxChannelCfg->transferFlags(DMAConfig::FORCE_DI_OFF);
		}

		//prepare rx channel
		dma->Setup(rxChannelCfg);

    	return true;
    }

    static bool isTransferBusy() {
    	return running;
    }

	static bool
	isTransferFinished() {
		return !running;
	}

protected:
	static DMAConfig* txChannelCfg;
	static DMAConfig* rxChannelCfg;
	volatile static bool running;
private:

	static void onDMAComplete() {
		SPIx->DMACR = 0;
		running = false;
	}
};

template<int Spi_ptr>
DMAConfig* SpiMaster<Spi_ptr>::txChannelCfg = 0;

template<int Spi_ptr>
DMAConfig* SpiMaster<Spi_ptr>::rxChannelCfg = 0;

template<int Spi_ptr>
volatile bool SpiMaster<Spi_ptr>::running = 0;

typedef SpiMaster<(int)LPC_SSP0> SpiMaster0;
typedef SpiMaster<(int)LPC_SSP1> SpiMaster1;


} // lpc namespace
} // xpcc namespace

#endif
