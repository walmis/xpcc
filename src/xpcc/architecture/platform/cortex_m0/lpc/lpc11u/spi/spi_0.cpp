
/*
 * WARNING: This file is generated automatically, do not edit!
 * Please modify the corresponding *.in file instead and rebuild this file. 
 */
// ----------------------------------------------------------------------------

#include "../../device.h"
#include "spi_0.hpp"


// ----------------------------------------------------------------------------
uint8_t
xpcc::lpc11u::SpiMaster0::write(uint8_t data)
{
	/* Move on only if TX FIFO not full. */
	while (!(LPC_SSP0->SR & SPI_SRn_TNF));

	/* Put data into FIFO */
	LPC_SSP0->DR = data;
	
	/* Wait until data is received */
	while (LPC_SSP0->SR & SPI_SRn_BSY);
	
	return LPC_SSP0->DR;
}
