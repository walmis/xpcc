#ifndef XPCC_LPC11__UART_1_HPP
#define XPCC_LPC11__UART_1_HPP

#include <stdint.h>
#include <stddef.h>
#include "lpc11_uart_registers.hpp"
//lpc17xx uses the same uart api
#include <xpcc/architecture/platform/cortex_m3/lpc/lpc17/buffered_uart.hpp>

namespace xpcc
{
	namespace lpc11
	{
		/**
		 * \brief	Universal asynchronous receiver transmitter (UART1)
		 *
		 * Simple implementation without interrupts.
		 * It is buffered in hardware with 16-bytes FIFOs (one for receive and
		 * one for transmit)
		 * 
		 * \ingroup	lpc1100
		 */


		class Uart1
		{
		public:
			typedef enum {
				UART_DATABIT_5		= 0,     		/*!< UART 5 bit data mode */
				UART_DATABIT_6,		     			/*!< UART 6 bit data mode */
				UART_DATABIT_7,		     			/*!< UART 7 bit data mode */
				UART_DATABIT_8		     			/*!< UART 8 bit data mode */
			} CfgDataBits;

			/**
			 * @brief UART Stop bit type definitions
			 */
			typedef enum {
				UART_STOPBIT_1		= (0),   					/*!< UART 1 Stop Bits Select */
				UART_STOPBIT_2		 							/*!< UART Two Stop Bits Select */
			} CfgStopBits;

			/**
			 * @brief UART Parity type definitions
			 */
			typedef enum {
				UART_PARITY_NONE 	= 0,					/*!< No parity */
				UART_PARITY_ODD,	 						/*!< Odd parity */
				UART_PARITY_EVEN, 							/*!< Even parity */
				UART_PARITY_SP_1, 							/*!< Forced "1" stick parity */
				UART_PARITY_SP_0 							/*!< Forced "0" stick parity */
			} CfgParity;

			enum RxFifoTriggerLevel {
				TRIGGER_1 	= 0<<6,					/*!< No parity */
				TRIGGER_4   = 1<<6,	 						/*!< Odd parity */
				TRIGGER_8   = 2<<6, 							/*!< Even parity */
				TRIGGER_14  = 3<<6, 							/*!< Forced "1" stick parity */
			} ;

			static void init(uint32_t baud = 115200,
					CfgDataBits dataBits = UART_DATABIT_8,
					CfgParity parity = UART_PARITY_NONE,
					CfgStopBits stopBits = UART_STOPBIT_1);

			//write a character (blocking)
			static size_t write(char c) {
				while(txBusy());
				put(c);
				return 1;
			}
			//read a character
			static int16_t read() {
				if(LPC_UART->LCR & LSR_RDR) {
					return get();
				}
				return -1;
			}
			//flush data
			static void flush() {
				while (txBusy());
			}


			static ALWAYS_INLINE bool txBusy() {
				return !((LPC_UART->LSR & LSR_THRE) && (LPC_UART->LSR & LSR_TEMT));
			}

			static ALWAYS_INLINE bool txEmpty() {
				return LPC_UART->LSR & LSR_THRE;
			}

			static ALWAYS_INLINE bool rxEmpty() {
				return !(LPC_UART->LSR & LSR_RDR);
			}

			//put char to fifo
			static inline void put(char c) {
				LPC_UART->THR = c;
			}
			//get char from fifo
			static inline char get() {
				return LPC_UART->RBR;
			}

			static void enableTxCompleteInterrupt(bool en) {
				if(en)
					LPC_UART->IER |= IER_THREIE;
				else
					LPC_UART->IER &= ~IER_THREIE;
			}

			static void enableRxCompleteInterrupt(bool en) {
				if(en)
					LPC_UART->IER |= IER_RBRIE;
				else
					LPC_UART->IER &= ~IER_RBRIE;
			}

			static void attachTxCompleteInterrupt(void (*f)()) {
				txCallback = f;
			}
			static void attachRxCompleteInterrupt(void (*f)()) {
				rxCallback = f;
			}

			static uint8_t getFifoSize() {
				return 16;
			}

			static bool setBaud(uint32_t baud);

			static void startAutoBaud(uint8_t mode);
			static void stopAutoBaud();
			static bool autoBaudSuccess();

			static void handleIRQ();

		protected:
			static void (*txCallback)();
			static void (*rxCallback)();

			static void
			configurePins(void);
		}; // Uart1 class


		typedef BufferedUart<Uart1> BufferedUart;

	} // lpc namespace
} // xpcc namespace

#endif // XPCC_LPC11__UART_1_HPP
