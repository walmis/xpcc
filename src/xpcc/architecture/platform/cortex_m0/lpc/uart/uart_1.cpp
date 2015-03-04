#include "../uart/uart_1.hpp"

#include <xpcc/architecture/driver/atomic.hpp>

#include "../device.h"


#include <xpcc_config.hpp>

// ----------------------------------------------------------------------------
namespace xpcc {
namespace lpc11 {

// ----------------------------------------------------------------------------

void (*Uart1::txCallback)();
void (*Uart1::rxCallback)();

void xpcc::lpc11::Uart1::init(uint32_t baud, CfgDataBits dataBits,
					CfgParity parity, CfgStopBits stopBits)
{
	/*  UART I/O config. PIO1_6 and PIO1_7 are always Rx/Tx */
#ifdef __ARM_LPC11XX__
	xpcc::lpc11::IOCon::setPinFunc(1, 6, 1);
	xpcc::lpc11::IOCon::setPinFunc(1, 7, 1);
#endif
#ifdef __ARM_LPC11UXX__
	xpcc::lpc11::IOCon::setPinFunc(0, 18, 1);
	xpcc::lpc11::IOCon::setPinFunc(0, 19, 1);
#endif

	/* Enable UART clock */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);
	LPC_SYSCON->UARTCLKDIV = 0x1;     /* divided by 1 */

	setBaud(baud);

	/* Enable and reset TX and RX FIFO. */
	LPC_UART->FCR = FCR_TXFIFORES | FCR_RXFIFORES | FCR_FIFOEN;

	/* Read to clear the line status. */
	uint32_t regVal = LPC_UART->LSR;
	(void)regVal; // unused, just read to clear.

	/* Ensure a clean start, no data in either TX or RX FIFO. */
	while (( LPC_UART->LSR & (LSR_THRE|LSR_TEMT)) != (LSR_THRE|LSR_TEMT) );
	while ( LPC_UART->LSR & LSR_RDR )
	{
		regVal = LPC_UART->RBR;	/* Dump data from RX FIFO */
	}

	NVIC_EnableIRQ(UART_IRQn);

} // Uart1 constructor

void Uart1::handleIRQ() {
	uint32_t intstat = LPC_UART->IIR;
	if(!(intstat & 1)) {
		switch(intstat & 0xF) {
		case IIR_INTID_THRE:
			if(txCallback != 0)
				txCallback();
			break;
		case IIR_INTID_RDA:
		case IIR_INTID_CTI:
			if(rxCallback != 0)
				rxCallback();
			else
				LPC_UART->RBR;
			break;
		}
	}
}

extern "C" void
UART_IRQHandler()
{
	Uart1::handleIRQ();
}

}
}
