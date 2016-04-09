// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2012, Roboterclub Aachen e.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Roboterclub Aachen e.V. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
// ----------------------------------------------------------------------------

#include "uart_base.hpp"
#include "lpc11_uart_registers.hpp"

#include "../device.h"

/* Accepted Error baud rate value (in percent unit) */
#define UART_ACCEPTED_BAUDRATE_ERROR	(3)			/*!< Acceptable UART baudrate error */

/*********************************************************************//**
 * Macro defines for Macro defines for UART Fractional divider register
 **********************************************************************/
#define UART_FDR_DIVADDVAL(n)	((uint32_t)(n&0x0F))		/**< Baud-rate generation pre-scaler divisor */
#define UART_FDR_MULVAL(n)		((uint32_t)((n<<4)&0xF0))	/**< Baud-rate pre-scaler multiplier value */
#define UART_FDR_BITMASK		((uint32_t)(0xFF))			/**< UART Fractional Divider register bit mask */

#define UART_LOAD_DLM(div)  (((div) >> 8) & 0xFF)	/**< Macro for loading most significant halfs of divisors */
#define UART_LOAD_DLL(div)	((div) & 0xFF)	/**< Macro for loading least significant halfs of divisors */

#define UART_LCR_DLAB_EN		((uint8_t)(1<<7))    	/*!< UART Divisor Latches Access bit enable */
#define UART_LCR_BITMASK		((uint8_t)(0xFF))		/*!< UART line control bit mask */


void xpcc::lpc11::Uart1::startAutoBaud(uint8_t mode) {
	LPC_UART->FDR = 1<<4; //clear DIVADDVAL and set MULVAL to 1

	LPC_UART->ACR = (0<<1) | ((mode & 1) << 1) | (1<<2); //start, auto restart
	LPC_UART->IER |= (1<<8); //enable autobaud int

}

bool xpcc::lpc11::Uart1::autoBaudSuccess() {
	if(LPC_UART->IIR & (1<<8)) {
		LPC_UART->ACR |= (1<<8); //clear interrupt
		return true;
	}
	return false;
}

void xpcc::lpc11::Uart1::stopAutoBaud()  {
	LPC_UART->ACR = 0;
}

bool
xpcc::lpc11::Uart1::setBaud(uint32_t baudrate)
{
//	LPC_UART->LCR = LCR_DLAB;             /* DLAB = 1 */
//	uint32_t regVal = LPC_SYSCON->UARTCLKDIV;
//	uint32_t   Fdiv = (((SystemCoreClock * LPC_SYSCON->SYSAHBCLKDIV)/regVal)/16)/baudrate ;	/*baud rate */
//	LPC_UART->DLM = Fdiv >>    8;
//	LPC_UART->DLL = Fdiv  & 0xff;
//
//	LPC_UART->FDR = 0b00010000;
//
//	LPC_UART->LCR = LCR_WLS_8_BIT;		/* 8 bits, no Parity, 1 Stop bit, DLAB = 0 */
	uint32_t uClk = ((SystemCoreClock * LPC_SYSCON->SYSAHBCLKDIV)/LPC_SYSCON->UARTCLKDIV)/16;

	uint32_t calcBaudrate = 0;
	uint32_t temp = 0;

	uint32_t mulFracDiv, dividerAddFracDiv;
	uint32_t diviser = 0 ;
	uint32_t mulFracDivOptimal = 1;
	uint32_t dividerAddOptimal = 0;
	uint32_t diviserOptimal = 0;

	uint32_t relativeError = 0;
	uint32_t relativeOptimalError = 100000;

	for (mulFracDiv = 1 ; mulFracDiv <= 15 ;mulFracDiv++)
		{
		for (dividerAddFracDiv = 0 ; dividerAddFracDiv <= 15 ;dividerAddFracDiv++)
		{
		  temp = (mulFracDiv * uClk) / ((mulFracDiv + dividerAddFracDiv));

		  diviser = temp / baudrate;
		  if ((temp % baudrate) > (baudrate / 2))
			diviser++;

		  if (diviser >= 1 && diviser < 65536)
		  {
			calcBaudrate = temp / diviser;

			if (calcBaudrate <= baudrate)
			  relativeError = baudrate - calcBaudrate;
			else
			  relativeError = calcBaudrate - baudrate;

			if ((relativeError < relativeOptimalError))
			{
			  mulFracDivOptimal = mulFracDiv ;
			  dividerAddOptimal = dividerAddFracDiv;
			  diviserOptimal = diviser;
			  relativeOptimalError = relativeError;
			  if (relativeError == 0)
				break;
			}
		  } /* End of if */
		} /* end of inner for loop */
		if (relativeError == 0)
		  break;
		} /* end of outer for loop  */

		if (relativeOptimalError < ((baudrate * UART_ACCEPTED_BAUDRATE_ERROR)/100))
		{

				LPC_UART->LCR |= LCR_DLAB;
				LPC_UART->/*DLIER.*/DLM = UART_LOAD_DLM(diviserOptimal);
				LPC_UART->/*RBTHDLR.*/DLL = UART_LOAD_DLL(diviserOptimal);
				/* Then reset DLAB bit */
				LPC_UART->LCR &= (~UART_LCR_DLAB_EN) & UART_LCR_BITMASK;
				LPC_UART->FDR = (UART_FDR_MULVAL(mulFracDivOptimal) \
						| UART_FDR_DIVADDVAL(dividerAddOptimal)) & UART_FDR_BITMASK;

		}

		LPC_UART->LCR = LCR_WLS_8_BIT;

		return true;
}
