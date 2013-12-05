#ifndef XPCC__SPI_REGISTERS_HPP
#define XPCC__SPI_REGISTERS_HPP

/*******************  Bit definition for SPI_CR0 register  ********************/
#define SPI_CR0_CPOL						((uint16_t)(1 <<  6))
#define SPI_CR0_CPHA						((uint16_t)(1 <<  7))

/*******************  Bit definition for SPI_CR1 register  ********************/
#define SPI_CR1_SSE							((uint16_t)(1 <<  1))
#define SPI_CR1_SLAVE						((uint16_t)(1 <<  2))

/*******************  Bit definition for SPI_SR0/1 register  ********************/
#define SPI_SRn_TFE		((uint16_t)(1 <<  0)) ///< Transmit FIFO Empty. This bit is 1 is the Transmit FIFO is empty, 0 if not.
#define SPI_SRn_TNF		((uint16_t)(1 <<  1)) ///< Transmit FIFO Not Full. This bit is 0 if the Tx FIFO is full, 1 if not.
#define SPI_SRn_RNE		((uint16_t)(1 <<  2)) ///< Receive FIFO Not Empty. This bit is 0 if the Receive FIFO is empty, 1 if not.
#define SPI_SRn_RFF		((uint16_t)(1 <<  3)) ///< Receive FIFO Full. This bit is 1 if the Receive FIFO is full, 0 if not.
#define SPI_SRn_BSY		((uint16_t)(1 <<  4)) ///< Busy. This bit is 0 if the SPI controller is idle, 1 if it is currentl

/*******************  Bit definition for PRESETCTRL register  *****************/
#define PRESETCTRL_SSP0_RST_N				((uint16_t)(1 <<  0)) ///< SPI0 reset de-asserted
#define PRESETCTRL_SSP1_RST_N				((uint16_t)(1 <<  2)) ///< SPI1 reset de-asserted

/*******************  Bit definition for SYSAHBCLKCTRL register  **************/
#define SYSAHBCLKCTRL_SSP0					((uint16_t)(1 << 11)) ///< Enables clock for SPI0
#define SYSAHBCLKCTRL_SSP1					((uint32_t)(1 << 18)) ///< Enables clock for SPI1


namespace xpcc {
namespace lpc11 {

	enum class TransferSize
	{
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

	enum class FrameFormat
	{
		SPI = 0x00,
		TI  = 0x01,
		MICROWIRE = 0x02,
	};

	enum class Mode
	{
		MODE_0 = 0,								///< SCK normal,   sample on rising  edge
		MODE_1 = SPI_CR0_CPHA,					///< SCK normal,   sample on falling edge
		MODE_2 = SPI_CR0_CPOL,					///< SCK inverted, sample on falling edge
		MODE_3 = SPI_CR0_CPOL | SPI_CR0_CPHA,	///< SCK inverted, sample on rising  edge
	};


}
}


#endif // XPCC__SPI_REGISTERS_HPP
