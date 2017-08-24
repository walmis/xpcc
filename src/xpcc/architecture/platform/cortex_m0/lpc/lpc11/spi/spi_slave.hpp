/*
 * spi_slave.hpp
 *
 *  Created on: Dec 4, 2013
 *      Author: walmis
 */

#ifndef SPI_SLAVE_HPP_
#define SPI_SLAVE_HPP_

#include "../../device.h"
#include "../../iocon.hpp"
#include "../../spi_registers.h"

namespace xpcc {
namespace lpc11 {

#define SSPx reinterpret_cast<LPC_SSP_TypeDef*>(_ssp)

template<int _ssp>
class SpiSlave {
public:
	static void
	ALWAYS_INLINE
	initialize(Mode mode = Mode::MODE_0,
			TransferSize transferSize = TransferSize::BIT_08,
			FrameFormat frameFormat = FrameFormat::SPI) {

		if(SSPx == LPC_SSP0) {
			// Enable peripheral clock

			LPC_SYSCON->SYSAHBCLKCTRL	|= SYSAHBCLKCTRL_SSP0;

			// Deassert Reset
			LPC_SYSCON->PRESETCTRL 		|= PRESETCTRL_SSP0_RST_N;

			// Divide peripheral clock by 1
			LPC_SYSCON->SSP0CLKDIV = 0x01;

		} else {
			LPC_SYSCON->SYSAHBCLKCTRL	|= SYSAHBCLKCTRL_SSP1;

			// Deassert Reset
			LPC_SYSCON->PRESETCTRL 		|= PRESETCTRL_SSP1_RST_N;

			// Divide peripheral clock by 1
			LPC_SYSCON->SSP1CLKDIV = 0x01;
		}


		SSPx->CR0 = (static_cast<uint16_t>(mode)) |
				((static_cast<uint16_t>(frameFormat)) << 4) |
				((static_cast<uint16_t>(transferSize)) << 0);

		for (uint8_t ii = 0; ii < 8; ++ii)
		{
			uint16_t Dummy = LPC_SSP0->DR; /* clear the RxFIFO */
			(void)Dummy; // unused
		}

		SSPx->CR1 = 0;

		SSPx->CR1 = SPI_CR1_SLAVE;
		SSPx->CR1 |= SPI_CR1_SSE;

	}

	static bool
	ALWAYS_INLINE
	read(uint16_t &data) {
		if(SSPx->SR & SPI_SRn_RNE) {
			data = SSPx->DR;
			return true;
		}
		return false;
	}

	static uint16_t
	ALWAYS_INLINE
	read() {
		while(!(SSPx->SR & SPI_SRn_RNE) && (SSPx->SR & SPI_SRn_BSY));

		if(SSPx->SR & SPI_SRn_RNE) {
			return SSPx->DR;
		}

		return 0;
	}

	static void
	ALWAYS_INLINE
	write(uint16_t data) {
		/* Move on only if TX FIFO not full. */
		while (!(SSPx->SR & SPI_SRn_TNF));

		/* Put data into FIFO */
		SSPx->DR = data;
	}

};

typedef SpiSlave<(int)LPC_SSP0_BASE> SpiSlave0;
typedef SpiSlave<(int)LPC_SSP1_BASE> SpiSlave1;

}
}



#endif /* SPI_SLAVE_HPP_ */
