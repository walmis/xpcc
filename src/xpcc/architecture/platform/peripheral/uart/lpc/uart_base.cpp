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

void
xpcc::lpc::UartBase::setBaudrate(uint32_t baudrate)
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

		  if (diviser > 2 && diviser < 65536)
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

}
