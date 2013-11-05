
/*
 * WARNING: This file is generated automatically, do not edit!
 * Please modify the corresponding *.in file instead and rebuild this file. 
 */
// ----------------------------------------------------------------------------

#include "../../device.h"
#include "spi_1.hpp"

namespace
{
	static uint8_t* transmitBuffer(0);
	static uint8_t* receiveBuffer(0);
	static uint16_t bufferLength(0);
	enum
	{
		BUFFER_TRANSMIT_INCR_bm = 0x01,
		BUFFER_RECEIVE_INCR_bm = 0x02,
		BUFFER_TRANSMIT_IS_NOT_ZERO_bm = 0x04,
		BUFFER_RECEIVE_IS_NOT_ZERO_bm = 0x08,
		BUFFER_IS_DUMMY_bm = 0x10,
		BUFFER_IS_BUSY_SYNC_bm = 0x20
	};
	static uint8_t status(0);
}


// ----------------------------------------------------------------------------
uint8_t
xpcc::lpc::SpiMaster1::write(uint8_t data)
{
	/* Move on only if TX FIFO not full. */
	while (!(LPC_SSP1->SR & SPI_SRn_TNF));

	/* Put data into FIFO */
	LPC_SSP1->DR = data;
	
	/* Wait until data is received */
	while (LPC_SSP1->SR & SPI_SRn_BSY);
	
	return LPC_SSP1->DR;
}
