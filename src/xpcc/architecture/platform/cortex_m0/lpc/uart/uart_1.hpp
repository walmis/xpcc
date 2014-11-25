#ifndef XPCC_LPC11__UART_1_HPP
#define XPCC_LPC11__UART_1_HPP

#include <stdint.h>
#include <stddef.h>

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

			static void init(uint32_t baud = 115200,
					CfgDataBits dataBits = UART_DATABIT_8,
					CfgParity parity = UART_PARITY_NONE,
					CfgStopBits stopBits = UART_STOPBIT_1);

			static size_t write(char c);
			static int16_t read();
			static void flush();


			static inline void put(char c);
			static inline char get();

			static void enableTxCompleteInterrupt(bool en);
			static void enableRxCompleteInterrupt(bool en);

			static void attachTxCompleteInterrupt(void (*f)());
			static void attachRxCompleteInterrupt(void (*f)());

			static bool setBaud(uint32_t baud);

			static void startAutoBaud(uint8_t mode);
			static void stopAutoBaud();
			static bool autoBaudSuccess();


		protected:

			static void
			configurePins(void);
		}; // Uart1 class


	} // lpc namespace
} // xpcc namespace

#endif // XPCC_LPC11__UART_1_HPP
