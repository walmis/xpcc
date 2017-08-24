/*
 * iap.hpp
 *
 *  Created on: Apr 15, 2013
 *      Author: walmis
 */

#ifndef IAP_HPP_
#define IAP_HPP_

#include "device.h"
#include "uart/lpc11_uart_registers.hpp"

namespace xpcc {
namespace lpc11 {


#define IAP_ADDRESS 0x1FFF1FF1

typedef enum
{
	PREPARE_SECTOR_FOR_WRITE	=50,
	COPY_RAM_TO_FLASH			=51,
	ERASE_SECTOR				=52,
	BLANK_CHECK_SECTOR			=53,
	READ_PART_ID				=54,
	READ_BOOT_VER				=55,
	COMPARE						=56,
	REINVOKE_ISP				=57,
	READ_UID				    =58,
}IAP_Command_Code;

class IAP {
public:
	static bool call(uint32_t param_tab[5], uint32_t result_tab[5]) {
		void (*iap)(uint32_t[], uint32_t[]);
		iap = (void (*)(uint32_t[], uint32_t[]))IAP_ADDRESS;

		iap(param_tab, result_tab);
		if(result_tab[0] == 0)
			return true;

		return false;

	}

	static void readUID(uint32_t result[4]) {
		uint32_t param_table[5];
		uint32_t result_table[5];

		param_table[0] = READ_UID;
		call(param_table, result_table);

		result[0] = result_table[1];
		result[1] = result_table[2];
		result[2] = result_table[3];
		result[3] = result_table[4];
	}

	static uint32_t writeFlash(unsigned flash_address, unsigned * flash_data_buf,
			unsigned count, uint32_t clk = SystemCoreClock / 1000) {
		__disable_irq();
		uint32_t param_table[5];
		uint32_t result_table[5];

		param_table[0] = COPY_RAM_TO_FLASH;
		param_table[1] = flash_address;
		param_table[2] = (unsigned) flash_data_buf;
		param_table[3] = count;
		param_table[4] = clk;
		call(param_table, result_table);
		__enable_irq();
		return result_table[0];
	}

	static void eraseSector(unsigned start_sector, unsigned end_sector,
			uint32_t clk = SystemCoreClock / 1000) {

		uint32_t param_table[5];
		uint32_t result_table[5];
		param_table[0] = ERASE_SECTOR;
		param_table[1] = start_sector;
		param_table[2] = end_sector;
		param_table[3] = clk;
		call(param_table, result_table);
	}

	static void prepareSector(unsigned start_sector, unsigned end_sector,
			uint32_t clk = SystemCoreClock / 1000) {

		uint32_t param_table[5];
		uint32_t result_table[5];
		param_table[0] = PREPARE_SECTOR_FOR_WRITE;
		param_table[1] = start_sector;
		param_table[2] = end_sector;
		param_table[3] = clk;
		call(param_table, result_table);
	}


	static void invokeISP(void) {
		uint32_t temp;

		/* Disable UART interrupts */
		LPC_UART->IER = 0;
		/* Disable UART interrupts in NVIC */
		//NVIC_DisableIRQ(UART_IRQn);
		NVIC->ICER[0] = 0xFFFFFFFF;

		SysTick->CTRL = (1<<2);

		/* Ensure a clean start, no data in either TX or RX FIFO. */
		while ((LPC_UART->LSR & (LSR_THRE | LSR_TEMT)) != (LSR_THRE | LSR_TEMT))
			;

		while (LPC_UART->LSR & LSR_RDR) {
			temp = LPC_UART->RBR; /* Dump data from RX FIFO */
		}

		/* Read to clear the line status. */
		temp = LPC_UART->LSR;
		(void)temp;

		typedef void (*IAP)(uint32_t[], uint32_t[]);
		IAP iap_entry = (IAP) 0x1fff1ff1;
		static uint32_t command[5], result[4];

//		LPC_SYSCON->PDRUNCFG &= ~(1 << 5); /* Power-up System Osc      */
//
//		LPC_SYSCON->SYSOSCCTRL = 0;
//		LPC_SYSCON->SYSPLLCLKSEL = 0; /* Select PLL Input         */
//
//		LPC_SYSCON->SYSPLLCLKUEN = 0x01; /* Update Clock Source      */
//		LPC_SYSCON->SYSPLLCLKUEN = 0x00; /* Toggle Update Register   */
//		LPC_SYSCON->SYSPLLCLKUEN = 0x01;
//		while (!(LPC_SYSCON->SYSPLLCLKUEN & 0x01))
//			; /* Wait Until Updated       */
//		/* System PLL Setup         */
//		LPC_SYSCON->SYSPLLCTRL = 0;
//		LPC_SYSCON->PDRUNCFG &= ~(1 << 7); /* Power-up SYSPLL          */
//		while (!(LPC_SYSCON->SYSPLLSTAT & 0x01))
//			; /* Wait Until PLL Locked    */
//
//		LPC_SYSCON->MAINCLKSEL = 0; /* Select PLL Clock Output  */
//		LPC_SYSCON->MAINCLKUEN = 0x01; /* Update MCLK Clock Source */
//		LPC_SYSCON->MAINCLKUEN = 0x00; /* Toggle Update Register   */
//		LPC_SYSCON->MAINCLKUEN = 0x01;
//		while (!(LPC_SYSCON->MAINCLKUEN & 0x01))
//			; /* Wait Until Updated       */

		LPC_SYSCON->UARTCLKDIV = 0;

		LPC_SYSCON->SYSMEMREMAP = 0;


		/* make sure 32-bit Timer 1 is turned on before calling ISP */
		LPC_SYSCON->SYSAHBCLKCTRL |= 0x00400;
		/* make sure GPIO clock is turned on before calling ISP */
		LPC_SYSCON->SYSAHBCLKCTRL |= 0x00040;
		/* make sure IO configuration clock is turned on before calling ISP */
		LPC_SYSCON->SYSAHBCLKCTRL |= 0x10000;
		/* make sure AHB clock divider is 1:1 */
		LPC_SYSCON->SYSAHBCLKDIV = 1;


		/* Send Reinvoke ISP command to ISP entry point*/
		command[0] = 57;

		/* Set stack pointer to ROM value (reset default).
		 This must be the last piece of code executed before calling ISP,
		 because most C expressions and function returns will fail after
		 the stack pointer is changed.
		 */
		__set_MSP(*((uint32_t *) 0x1FFF0000)); /* inline asm */

		/* Invoke ISP. We call "iap_entry" to invoke ISP because the ISP entry
		 is done through the same command interface as IAP. */
		iap_entry(command, result);

		// Code will never return!
	}


};
}
}

#endif /* IAP_HPP_ */
