
/*
 * WARNING: This file is generated automatically, do not edit!
 * Please modify the corresponding *.in file instead and rebuild this file. 
 */
// ----------------------------------------------------------------------------

#include "../../device.h"
#include "spi_0.hpp"

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
bool
xpcc::lpc::SpiMaster0::setBuffer(uint16_t length, uint8_t* transmit, uint8_t* receive, BufferIncrease bufferIncrease)
{
	if (!isFinished()) {
		return false;
	}
	
	transmitBuffer = transmit;
	receiveBuffer = receive ? receive : transmit;
	bufferLength = length;
	
	status &= ~(BUFFER_TRANSMIT_INCR_bm | BUFFER_RECEIVE_INCR_bm);
	status |= bufferIncrease;
	
	return true;
}

bool
xpcc::lpc::SpiMaster0::transfer(TransferOptions options)
{
	if (status & BUFFER_IS_BUSY_SYNC_bm) {
		return false;
	}
	
	uint8_t rx = 0;
	uint8_t tx = 0xff;
		
	// send the buffer out, blocking
	status |= BUFFER_IS_BUSY_SYNC_bm;
	
	// check if we have to use a dummy buffer
	bool transmit = (options & TRANSFER_SEND_BUFFER_DISCARD_RECEIVE) & static_cast<bool>(transmitBuffer);
	bool receive = (options & TRANSFER_SEND_DUMMY_SAVE_RECEIVE) & static_cast<bool>(receiveBuffer);
	
	// If this is not a write-only transfer
	if (receive) {
		// This is an SPI master, so all data in the Rx FIFO can be discarded
		// because they may result from a former write-only transfer.
		while (LPC_SSP0->SR & SPI_SRn_RNE) {
			uint16_t dummy = LPC_SSP0->DR;
			(void) dummy;
		}
	}
	
	while (bufferLength) 
	{
		// Put as many bytes of the buffer into the hardware Tx FIFO
		// until it is full or all bytes from the buffer were sent. 
		uint_fast16_t ii = 0;
		while ((LPC_SSP0->SR & SPI_SRn_TNF) && (ii < bufferLength))
		{
			if (transmit) {
				tx = transmitBuffer[(status & BUFFER_TRANSMIT_INCR_bm) ? ii : bufferLength-1-ii];
			}
			LPC_SSP0->DR = tx;
			++ii;
		}
		
		// ii is the number of bytes just written to FIFO
		// For every byte written one byte is received. 
		
		// Get corresponding number of bits from Rx FIFO if this is not a
		// Tx-only transfer
		if (receive) {
			uint16_t jj = 0; // number of bytes received. 
			while (jj < ii) {
				// Busy-waiting for data until all data received.
				while (!(LPC_SSP0->SR & SPI_SRn_RNE));
				receiveBuffer[(status & BUFFER_RECEIVE_INCR_bm) ? jj : bufferLength-1-jj] = LPC_SSP0->DR;
				++jj;
			}
		}
		
		// Block until all transfers finished
		// TODO Do not block here, use interrupts
		while (LPC_SSP0->SR & SPI_SRn_BSY);
		
		// Finished?
		if (ii >= bufferLength) {
			break;
		}	
	}

	// TODO Do not block here, use interrupt to clear BUFFER_IS_BUSY_SYNC_bm
	// flag when SPI is not busy any more
	while (LPC_SSP0->SR & SPI_SRn_BSY);
	status &= ~BUFFER_IS_BUSY_SYNC_bm;
	
	return true;
}

bool
xpcc::lpc::SpiMaster0::transferSync(TransferOptions options)
{
	return transfer(options);
}

bool
xpcc::lpc::SpiMaster0::isFinished()
{
	return !(status & BUFFER_IS_BUSY_SYNC_bm);
}

// ----------------------------------------------------------------------------
uint8_t
xpcc::lpc::SpiMaster0::write(uint8_t data)
{
	/* Move on only if TX FIFO not full. */
	while (!(LPC_SSP0->SR & SPI_SRn_TNF));

	/* Put data into FIFO */
	LPC_SSP0->DR = data;
	
	/* Wait until data is received */
	while (LPC_SSP0->SR & SPI_SRn_BSY);
	
	return LPC_SSP0->DR;
}
