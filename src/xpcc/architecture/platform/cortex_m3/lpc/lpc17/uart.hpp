#pragma once

#include "../lpc17xx.hpp"

namespace xpcc {
namespace lpc17 {

/* --------------------- BIT DEFINITIONS -------------------------------------- */
/*********************************************************************//**
 * Macro defines for Macro defines for UARTn Receiver Buffer Register
 **********************************************************************/
#define UART_RBR_MASKBIT   	((uint8_t)0xFF) 		/*!< UART Received Buffer mask bit (8 bits) */

/*********************************************************************//**
 * Macro defines for Macro defines for UARTn Transmit Holding Register
 **********************************************************************/
#define UART_THR_MASKBIT   	((uint8_t)0xFF) 		/*!< UART Transmit Holding mask bit (8 bits) */

/*********************************************************************//**
 * Macro defines for Macro defines for UARTn Divisor Latch LSB register
 **********************************************************************/
#define UART_LOAD_DLL(div)	((div) & 0xFF)	/**< Macro for loading least significant halfs of divisors */
#define UART_DLL_MASKBIT	((uint8_t)0xFF)	/*!< Divisor latch LSB bit mask */

/*********************************************************************//**
 * Macro defines for Macro defines for UARTn Divisor Latch MSB register
 **********************************************************************/
#define UART_DLM_MASKBIT	((uint8_t)0xFF)			/*!< Divisor latch MSB bit mask */
#define UART_LOAD_DLM(div)  (((div) >> 8) & 0xFF)	/**< Macro for loading most significant halfs of divisors */

/*********************************************************************//**
 * Macro defines for Macro defines for UART interrupt enable register
 **********************************************************************/
#define UART_IER_RBRINT_EN		((uint32_t)(1<<0)) 	/*!< RBR Interrupt enable*/
#define UART_IER_THREINT_EN		((uint32_t)(1<<1)) 	/*!< THR Interrupt enable*/
#define UART_IER_RLSINT_EN		((uint32_t)(1<<2)) 	/*!< RX line status interrupt enable*/
#define UART1_IER_MSINT_EN		((uint32_t)(1<<3))	/*!< Modem status interrupt enable */
#define UART1_IER_CTSINT_EN		((uint32_t)(1<<7))	/*!< CTS1 signal transition interrupt enable */
#define UART_IER_ABEOINT_EN		((uint32_t)(1<<8)) 	/*!< Enables the end of auto-baud interrupt */
#define UART_IER_ABTOINT_EN		((uint32_t)(1<<9)) 	/*!< Enables the auto-baud time-out interrupt */
#define UART_IER_BITMASK		((uint32_t)(0x307)) /*!< UART interrupt enable register bit mask */
#define UART1_IER_BITMASK		((uint32_t)(0x38F)) /*!< UART1 interrupt enable register bit mask */

/*********************************************************************//**
 * Macro defines for Macro defines for UART interrupt identification register
 **********************************************************************/
#define UART_IIR_INTSTAT_PEND	((uint32_t)(1<<0))	/*!<Interrupt Status - Active low */
#define UART_IIR_INTID_RLS		((uint32_t)(3<<1)) 	/*!<Interrupt identification: Receive line status*/
#define UART_IIR_INTID_RDA		((uint32_t)(2<<1)) 	/*!<Interrupt identification: Receive data available*/
#define UART_IIR_INTID_CTI		((uint32_t)(6<<1)) 	/*!<Interrupt identification: Character time-out indicator*/
#define UART_IIR_INTID_THRE		((uint32_t)(1<<1)) 	/*!<Interrupt identification: THRE interrupt*/
#define UART1_IIR_INTID_MODEM	((uint32_t)(0<<1)) 	/*!<Interrupt identification: Modem interrupt*/
#define UART_IIR_INTID_MASK		((uint32_t)(7<<1))	/*!<Interrupt identification: Interrupt ID mask */
#define UART_IIR_FIFO_EN		((uint32_t)(3<<6)) 	/*!<These bits are equivalent to UnFCR[0] */
#define UART_IIR_ABEO_INT		((uint32_t)(1<<8)) 	/*!< End of auto-baud interrupt */
#define UART_IIR_ABTO_INT		((uint32_t)(1<<9)) 	/*!< Auto-baud time-out interrupt */
#define UART_IIR_BITMASK		((uint32_t)(0x3CF))	/*!< UART interrupt identification register bit mask */

/*********************************************************************//**
 * Macro defines for Macro defines for UART FIFO control register
 **********************************************************************/
#define UART_FCR_FIFO_EN		((uint8_t)(1<<0)) 	/*!< UART FIFO enable */
#define UART_FCR_RX_RS			((uint8_t)(1<<1)) 	/*!< UART FIFO RX reset */
#define UART_FCR_TX_RS			((uint8_t)(1<<2)) 	/*!< UART FIFO TX reset */
#define UART_FCR_DMAMODE_SEL 	((uint8_t)(1<<3)) 	/*!< UART DMA mode selection */
#define UART_FCR_TRG_LEV0		((uint8_t)(0)) 		/*!< UART FIFO trigger level 0: 1 character */
#define UART_FCR_TRG_LEV1		((uint8_t)(1<<6)) 	/*!< UART FIFO trigger level 1: 4 character */
#define UART_FCR_TRG_LEV2		((uint8_t)(2<<6)) 	/*!< UART FIFO trigger level 2: 8 character */
#define UART_FCR_TRG_LEV3		((uint8_t)(3<<6)) 	/*!< UART FIFO trigger level 3: 14 character */
#define UART_FCR_BITMASK		((uint8_t)(0xCF))	/*!< UART FIFO control bit mask */
#define UART_TX_FIFO_SIZE		(16)

/*********************************************************************//**
 * Macro defines for Macro defines for UART line control register
 **********************************************************************/
#define UART_LCR_WLEN5     		((uint8_t)(0))   		/*!< UART 5 bit data mode */
#define UART_LCR_WLEN6     		((uint8_t)(1<<0))   	/*!< UART 6 bit data mode */
#define UART_LCR_WLEN7     		((uint8_t)(2<<0))   	/*!< UART 7 bit data mode */
#define UART_LCR_WLEN8     		((uint8_t)(3<<0))   	/*!< UART 8 bit data mode */
#define UART_LCR_STOPBIT_SEL	((uint8_t)(1<<2))   	/*!< UART Two Stop Bits Select */
#define UART_LCR_PARITY_EN		((uint8_t)(1<<3))		/*!< UART Parity Enable */
#define UART_LCR_PARITY_ODD		((uint8_t)(0))         	/*!< UART Odd Parity Select */
#define UART_LCR_PARITY_EVEN	((uint8_t)(1<<4))		/*!< UART Even Parity Select */
#define UART_LCR_PARITY_F_1		((uint8_t)(2<<4))		/*!< UART force 1 stick parity */
#define UART_LCR_PARITY_F_0		((uint8_t)(3<<4))		/*!< UART force 0 stick parity */
#define UART_LCR_BREAK_EN		((uint8_t)(1<<6))		/*!< UART Transmission Break enable */
#define UART_LCR_DLAB_EN		((uint8_t)(1<<7))    	/*!< UART Divisor Latches Access bit enable */
#define UART_LCR_BITMASK		((uint8_t)(0xFF))		/*!< UART line control bit mask */

/*********************************************************************//**
 * Macro defines for Macro defines for UART1 Modem Control Register
 **********************************************************************/
#define UART1_MCR_DTR_CTRL		((uint8_t)(1<<0))		/*!< Source for modem output pin DTR */
#define UART1_MCR_RTS_CTRL		((uint8_t)(1<<1))		/*!< Source for modem output pin RTS */
#define UART1_MCR_LOOPB_EN		((uint8_t)(1<<4))		/*!< Loop back mode select */
#define UART1_MCR_AUTO_RTS_EN	((uint8_t)(1<<6))		/*!< Enable Auto RTS flow-control */
#define UART1_MCR_AUTO_CTS_EN	((uint8_t)(1<<7))		/*!< Enable Auto CTS flow-control */
#define UART1_MCR_BITMASK		((uint8_t)(0x0F3))		/*!< UART1 bit mask value */

/*********************************************************************//**
 * Macro defines for Macro defines for UART line status register
 **********************************************************************/
#define UART_LSR_RDR		((uint8_t)(1<<0)) 	/*!<Line status register: Receive data ready*/
#define UART_LSR_OE			((uint8_t)(1<<1)) 	/*!<Line status register: Overrun error*/
#define UART_LSR_PE			((uint8_t)(1<<2)) 	/*!<Line status register: Parity error*/
#define UART_LSR_FE			((uint8_t)(1<<3)) 	/*!<Line status register: Framing error*/
#define UART_LSR_BI			((uint8_t)(1<<4)) 	/*!<Line status register: Break interrupt*/
#define UART_LSR_THRE		((uint8_t)(1<<5)) 	/*!<Line status register: Transmit holding register empty*/
#define UART_LSR_TEMT		((uint8_t)(1<<6)) 	/*!<Line status register: Transmitter empty*/
#define UART_LSR_RXFE		((uint8_t)(1<<7)) 	/*!<Error in RX FIFO*/
#define UART_LSR_BITMASK	((uint8_t)(0xFF)) 	/*!<UART Line status bit mask */

/*********************************************************************//**
 * Macro defines for Macro defines for UART Modem (UART1 only) status register
 **********************************************************************/
#define UART1_MSR_DELTA_CTS		((uint8_t)(1<<0))	/*!< Set upon state change of input CTS */
#define UART1_MSR_DELTA_DSR		((uint8_t)(1<<1))	/*!< Set upon state change of input DSR */
#define UART1_MSR_LO2HI_RI		((uint8_t)(1<<2))	/*!< Set upon low to high transition of input RI */
#define UART1_MSR_DELTA_DCD		((uint8_t)(1<<3))	/*!< Set upon state change of input DCD */
#define UART1_MSR_CTS			((uint8_t)(1<<4))	/*!< Clear To Send State */
#define UART1_MSR_DSR			((uint8_t)(1<<5))	/*!< Data Set Ready State */
#define UART1_MSR_RI			((uint8_t)(1<<6))	/*!< Ring Indicator State */
#define UART1_MSR_DCD			((uint8_t)(1<<7))	/*!< Data Carrier Detect State */
#define UART1_MSR_BITMASK		((uint8_t)(0xFF))	/*!< MSR register bit-mask value */

/*********************************************************************//**
 * Macro defines for Macro defines for UART Scratch Pad Register
 **********************************************************************/
#define UART_SCR_BIMASK		((uint8_t)(0xFF))	/*!< UART Scratch Pad bit mask */

/*********************************************************************//**
 * Macro defines for Macro defines for UART Auto baudrate control register
 **********************************************************************/
#define UART_ACR_START				((uint32_t)(1<<0))	/**< UART Auto-baud start */
#define UART_ACR_MODE				((uint32_t)(1<<1))	/**< UART Auto baudrate Mode 1 */
#define UART_ACR_AUTO_RESTART		((uint32_t)(1<<2))	/**< UART Auto baudrate restart */
#define UART_ACR_ABEOINT_CLR		((uint32_t)(1<<8))	/**< UART End of auto-baud interrupt clear */
#define UART_ACR_ABTOINT_CLR		((uint32_t)(1<<9))	/**< UART Auto-baud time-out interrupt clear */
#define UART_ACR_BITMASK			((uint32_t)(0x307))	/**< UART Auto Baudrate register bit mask */

/*********************************************************************//**
 * Macro defines for Macro defines for UART IrDA control register
 **********************************************************************/
#define UART_ICR_IRDAEN			((uint32_t)(1<<0))			/**< IrDA mode enable */
#define UART_ICR_IRDAINV		((uint32_t)(1<<1))			/**< IrDA serial input inverted */
#define UART_ICR_FIXPULSE_EN	((uint32_t)(1<<2))			/**< IrDA fixed pulse width mode */
#define UART_ICR_PULSEDIV(n)	((uint32_t)((n&0x07)<<3))	/**< PulseDiv - Configures the pulse when FixPulseEn = 1 */
#define UART_ICR_BITMASK		((uint32_t)(0x3F))			/*!< UART IRDA bit mask */

/*********************************************************************//**
 * Macro defines for Macro defines for UART Fractional divider register
 **********************************************************************/
#define UART_FDR_DIVADDVAL(n)	((uint32_t)(n&0x0F))		/**< Baud-rate generation pre-scaler divisor */
#define UART_FDR_MULVAL(n)		((uint32_t)((n<<4)&0xF0))	/**< Baud-rate pre-scaler multiplier value */
#define UART_FDR_BITMASK		((uint32_t)(0xFF))			/**< UART Fractional Divider register bit mask */

/*********************************************************************//**
 * Macro defines for Macro defines for UART Tx Enable register
 **********************************************************************/
#define UART_TER_TXEN			((uint8_t)(1<<7)) 		/*!< Transmit enable bit */
#define UART_TER_BITMASK		((uint8_t)(0x80))		/**< UART Transmit Enable Register bit mask */

/*********************************************************************//**
 * Macro defines for Macro defines for UART1 RS485 Control register
 **********************************************************************/
#define UART1_RS485CTRL_NMM_EN		((uint32_t)(1<<0))	/*!< RS-485/EIA-485 Normal Multi-drop Mode (NMM)
														is disabled */
#define UART1_RS485CTRL_RX_DIS		((uint32_t)(1<<1))	/*!< The receiver is disabled */
#define UART1_RS485CTRL_AADEN		((uint32_t)(1<<2))	/*!< Auto Address Detect (AAD) is enabled */
#define UART1_RS485CTRL_SEL_DTR		((uint32_t)(1<<3))	/*!< If direction control is enabled
														(bit DCTRL = 1), pin DTR is used for direction control */
#define UART1_RS485CTRL_DCTRL_EN	((uint32_t)(1<<4))	/*!< Enable Auto Direction Control */
#define UART1_RS485CTRL_OINV_1		((uint32_t)(1<<5))	/*!< This bit reverses the polarity of the direction
														control signal on the RTS (or DTR) pin. The direction control pin
														will be driven to logic "1" when the transmitter has data to be sent */
#define UART1_RS485CTRL_BITMASK		((uint32_t)(0x3F))	/**< RS485 control bit-mask value */

/*********************************************************************//**
 * Macro defines for Macro defines for UART1 RS-485 Address Match register
 **********************************************************************/
#define UART1_RS485ADRMATCH_BITMASK ((uint8_t)(0xFF)) 	/**< Bit mask value */

/*********************************************************************//**
 * Macro defines for Macro defines for UART1 RS-485 Delay value register
 **********************************************************************/
/* Macro defines for UART1 RS-485 Delay value register */
#define UART1_RS485DLY_BITMASK		((uint8_t)(0xFF)) 	/** Bit mask value */

/*********************************************************************//**
 * Macro defines for Macro defines for UART FIFO Level register
 **********************************************************************/
#define UART_FIFOLVL_RXFIFOLVL(n)	((uint32_t)(n&0x0F))		/**< Reflects the current level of the UART receiver FIFO */
#define UART_FIFOLVL_TXFIFOLVL(n)	((uint32_t)((n>>8)&0x0F))	/**< Reflects the current level of the UART transmitter FIFO */
#define UART_FIFOLVL_BITMASK		((uint32_t)(0x0F0F))		/**< UART FIFO Level Register bit mask */


/* ---------------- CHECK PARAMETER DEFINITIONS ---------------------------- */

/** Macro to check the input UART_DATABIT parameters */
#define PARAM_UART_DATABIT(databit)	((databit==UART_DATABIT_5) || (databit==UART_DATABIT_6)\
|| (databit==UART_DATABIT_7) || (databit==UART_DATABIT_8))

/** Macro to check the input UART_STOPBIT parameters */
#define PARAM_UART_STOPBIT(stopbit)	((stopbit==UART_STOPBIT_1) || (stopbit==UART_STOPBIT_2))

/** Macro to check the input UART_PARITY parameters */
#define PARAM_UART_PARITY(parity)	((parity==UART_PARITY_NONE) || (parity==UART_PARITY_ODD) \
|| (parity==UART_PARITY_EVEN) || (parity==UART_PARITY_SP_1) \
|| (parity==UART_PARITY_SP_0))

/** Macro to check the input UART_FIFO parameters */
#define PARAM_UART_FIFO_LEVEL(fifo)	((fifo==UART_FIFO_TRGLEV0) \
|| (fifo==UART_FIFO_TRGLEV1) || (fifo==UART_FIFO_TRGLEV2) \
|| (fifo==UART_FIFO_TRGLEV3))

/** Macro to check the input UART_INTCFG parameters */
#define PARAM_UART_INTCFG(IntCfg)	((IntCfg==UART_INTCFG_RBR) || (IntCfg==UART_INTCFG_THRE) \
|| (IntCfg==UART_INTCFG_RLS) || (IntCfg==UART_INTCFG_ABEO) \
|| (IntCfg==UART_INTCFG_ABTO))

/** Macro to check the input UART1_INTCFG parameters - expansion input parameter for UART1 */
#define PARAM_UART1_INTCFG(IntCfg)	((IntCfg==UART1_INTCFG_MS) || (IntCfg==UART1_INTCFG_CTS))

/** Macro to check the input UART_AUTOBAUD_MODE parameters */
#define PARAM_UART_AUTOBAUD_MODE(ABmode)	((ABmode==UART_AUTOBAUD_MODE0) || (ABmode==UART_AUTOBAUD_MODE1))

/** Macro to check the input UART_AUTOBAUD_INTSTAT parameters */
#define PARAM_UART_AUTOBAUD_INTSTAT(ABIntStat)	((ABIntStat==UART_AUTOBAUD_INTSTAT_ABEO) || \
		(ABIntStat==UART_AUTOBAUD_INTSTAT_ABTO))

/** Macro to check the input UART_IrDA_PULSEDIV parameters */
#define PARAM_UART_IrDA_PULSEDIV(PulseDiv)	((PulseDiv==UART_IrDA_PULSEDIV2) || (PulseDiv==UART_IrDA_PULSEDIV4) \
|| (PulseDiv==UART_IrDA_PULSEDIV8) || (PulseDiv==UART_IrDA_PULSEDIV16) \
|| (PulseDiv==UART_IrDA_PULSEDIV32) || (PulseDiv==UART_IrDA_PULSEDIV64) \
|| (PulseDiv==UART_IrDA_PULSEDIV128) || (PulseDiv==UART_IrDA_PULSEDIV256))

/* Macro to check the input UART1_SignalState parameters */
#define PARAM_UART1_SIGNALSTATE(x) ((x==INACTIVE) || (x==ACTIVE))

/** Macro to check the input PARAM_UART1_MODEM_PIN parameters */
#define PARAM_UART1_MODEM_PIN(x) ((x==UART1_MODEM_PIN_DTR) || (x==UART1_MODEM_PIN_RTS))

/** Macro to check the input PARAM_UART1_MODEM_MODE parameters */
#define PARAM_UART1_MODEM_MODE(x) ((x==UART1_MODEM_MODE_LOOPBACK) || (x==UART1_MODEM_MODE_AUTO_RTS) \
|| (x==UART1_MODEM_MODE_AUTO_CTS))

/** Macro to check the direction control pin type */
#define PARAM_UART_RS485_DIRCTRL_PIN(x)	((x==UART1_RS485_DIRCTRL_RTS) || (x==UART1_RS485_DIRCTRL_DTR))

/* Macro to determine if it is valid UART port number */
#define PARAM_UARTx(x)	((((uint32_t *)x)==((uint32_t *)LPC_UART0)) \
|| (((uint32_t *)x)==((uint32_t *)LPC_UART1)) \
|| (((uint32_t *)x)==((uint32_t *)LPC_UART2)) \
|| (((uint32_t *)x)==((uint32_t *)LPC_UART3)))
#define PARAM_UART_IrDA(x) (((uint32_t *)x)==((uint32_t *)LPC_UART3))
#define PARAM_UART1_MODEM(x) (((uint32_t *)x)==((uint32_t *)LPC_UART1))

/** Macro to check the input value for UART1_RS485_CFG_MATCHADDRVALUE parameter */
#define PARAM_UART1_RS485_CFG_MATCHADDRVALUE(x) ((x<0xFF))

/** Macro to check the input value for UART1_RS485_CFG_DELAYVALUE parameter */
#define PARAM_UART1_RS485_CFG_DELAYVALUE(x) ((x<0xFF))

/**
 * @}
 */


/* Public Types --------------------------------------------------------------- */
/** @defgroup UART_Public_Types UART Public Types
 * @{
 */

/* Accepted Error baud rate value (in percent unit) */
#define UART_ACCEPTED_BAUDRATE_ERROR	(3)			/*!< Acceptable UART baudrate error */

#define UARTx (reinterpret_cast<LPC_UART_TypeDef*>(P_UART))

template <int P_UART>
class Uart {
public:
	constexpr static LPC_UART_TypeDef* const uart = (LPC_UART_TypeDef*)(P_UART);
	/**
	 * @brief UART Databit type definitions
	 */
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

	/**
	 * @brief FIFO Level type definitions
	 */
	typedef enum {
		UART_FIFO_TRGLEV0 = 0,	/*!< UART FIFO trigger level 0: 1 character */
		UART_FIFO_TRGLEV1, 		/*!< UART FIFO trigger level 1: 4 character */
		UART_FIFO_TRGLEV2,		/*!< UART FIFO trigger level 2: 8 character */
		UART_FIFO_TRGLEV3		/*!< UART FIFO trigger level 3: 14 character */
	} CfgFifoLevel;

	/********************************************************************//**
	* @brief UART Interrupt Type definitions
	**********************************************************************/
	typedef enum {
		UART_INTCFG_RBR = 0,	/*!< RBR Interrupt enable*/
		UART_INTCFG_THRE,		/*!< THR Interrupt enable*/
		UART_INTCFG_RLS,		/*!< RX line status interrupt enable*/
		UART1_INTCFG_MS,		/*!< Modem status interrupt enable (UART1 only) */
		UART1_INTCFG_CTS,		/*!< CTS1 signal transition interrupt enable (UART1 only) */
		UART_INTCFG_ABEO,		/*!< Enables the end of auto-baud interrupt */
		UART_INTCFG_ABTO		/*!< Enables the auto-baud time-out interrupt */
	} CfgIntType;

	/**
	 * @brief UART Line Status Type definition
	 */
	typedef enum {
		UART_LINESTAT_RDR	= UART_LSR_RDR,			/*!<Line status register: Receive data ready*/
		UART_LINESTAT_OE	= UART_LSR_OE,			/*!<Line status register: Overrun error*/
		UART_LINESTAT_PE	= UART_LSR_PE,			/*!<Line status register: Parity error*/
		UART_LINESTAT_FE	= UART_LSR_FE,			/*!<Line status register: Framing error*/
		UART_LINESTAT_BI	= UART_LSR_BI,			/*!<Line status register: Break interrupt*/
		UART_LINESTAT_THRE	= UART_LSR_THRE,		/*!<Line status register: Transmit holding register empty*/
		UART_LINESTAT_TEMT	= UART_LSR_TEMT,		/*!<Line status register: Transmitter empty*/
		UART_LINESTAT_RXFE	= UART_LSR_RXFE			/*!<Error in RX FIFO*/
	} LineStat;

	ALWAYS_INLINE
	static void init(uint32_t baud = 115200,
			CfgDataBits dataBits = UART_DATABIT_8,
			CfgParity parity = UART_PARITY_NONE,
			CfgStopBits stopBits = UART_STOPBIT_1) {

		uint32_t tmp;

		if(UARTx == LPC_UART0)
		{
			/* Set up clock and power for UART module */
			CLKPwr::setClkPower(CLKPwr::PType::PCUART0, true);
			NVIC_EnableIRQ(UART0_IRQn);
		}

		if(((LPC_UART1_TypeDef *)UARTx) == LPC_UART1)
		{
			/* Set up clock and power for UART module */
			CLKPwr::setClkPower(CLKPwr::PType::PCUART1, true);
			NVIC_EnableIRQ(UART1_IRQn);
		}

		if(UARTx == LPC_UART2)
		{
			/* Set up clock and power for UART module */
			CLKPwr::setClkPower(CLKPwr::PType::PCUART2, true);
			NVIC_EnableIRQ(UART2_IRQn);
		}


		if(UARTx == LPC_UART3)
		{
			/* Set up clock and power for UART module */
			CLKPwr::setClkPower(CLKPwr::PType::PCUART3, true);
			NVIC_EnableIRQ(UART3_IRQn);
		}


		if (((LPC_UART1_TypeDef *)UARTx) == LPC_UART1)
		{
			/* FIFOs are empty */
			((LPC_UART1_TypeDef *)UARTx)->FCR = ( UART_FCR_FIFO_EN
					| UART_FCR_RX_RS | UART_FCR_TX_RS);

			// Dummy reading
			while (((LPC_UART1_TypeDef *)UARTx)->LSR & UART_LSR_RDR)
			{
				tmp = ((LPC_UART1_TypeDef *)UARTx)->/*RBTHDLR.*/RBR;
			}

			((LPC_UART1_TypeDef *)UARTx)->TER = UART_TER_TXEN;
			// Wait for current transmit complete
			while (!(((LPC_UART1_TypeDef *)UARTx)->LSR & UART_LSR_THRE));

			// Disable interrupt
			((LPC_UART1_TypeDef *)UARTx)->/*DLIER.*/IER = 0;
			// Set LCR to default state
			((LPC_UART1_TypeDef *)UARTx)->LCR = 0;
			// Set ACR to default state
			((LPC_UART1_TypeDef *)UARTx)->ACR = 0;
			// Set Modem Control to default state
			((LPC_UART1_TypeDef *)UARTx)->MCR = 0;
			// Set RS485 control to default state
			((LPC_UART1_TypeDef *)UARTx)->RS485CTRL = 0;
			// Set RS485 delay timer to default state
			((LPC_UART1_TypeDef *)UARTx)->RS485DLY = 0;
			// Set RS485 addr match to default state
			((LPC_UART1_TypeDef *)UARTx)->ADRMATCH = 0;
			//Dummy Reading to Clear Status
			tmp = ((LPC_UART1_TypeDef *)UARTx)->MSR;
			tmp = ((LPC_UART1_TypeDef *)UARTx)->LSR;
		}
		else
		{
			/* FIFOs are empty */
			UARTx->FCR = ( UART_FCR_FIFO_EN | UART_FCR_RX_RS | UART_FCR_TX_RS | UART_FCR_TRG_LEV3);


			// Dummy reading
			while (UARTx->LSR & UART_LSR_RDR)
			{
				tmp = UARTx->/*RBTHDLR.*/RBR;
			}

			UARTx->TER = UART_TER_TXEN;
			// Wait for current transmit complete
			while (!(UARTx->LSR & UART_LSR_THRE));

			// Disable interrupt
			UARTx->/*DLIER.*/IER = 0;
			// Set LCR to default state
			UARTx->LCR = 0;
			// Set ACR to default state
			UARTx->ACR = 0;
			// Dummy reading
			tmp = UARTx->LSR;
		}

		if (UARTx == LPC_UART3)
		{
			// Set IrDA to default state
			UARTx->ICR = 0;
		}

		// Set Line Control register ----------------------------

		uart_set_divisors(baud);

		if (((LPC_UART1_TypeDef *)UARTx) == LPC_UART1)
		{
			tmp = (((LPC_UART1_TypeDef *)UARTx)->LCR & (UART_LCR_DLAB_EN | UART_LCR_BREAK_EN)) \
					& UART_LCR_BITMASK;
		}
		else
		{
			tmp = (UARTx->LCR & (UART_LCR_DLAB_EN | UART_LCR_BREAK_EN)) & UART_LCR_BITMASK;
		}

		switch (dataBits){
		case UART_DATABIT_5:
			tmp |= UART_LCR_WLEN5;
			break;
		case UART_DATABIT_6:
			tmp |= UART_LCR_WLEN6;
			break;
		case UART_DATABIT_7:
			tmp |= UART_LCR_WLEN7;
			break;
		case UART_DATABIT_8:
		default:
			tmp |= UART_LCR_WLEN8;
			break;
		}

		if (parity == UART_PARITY_NONE)
		{
			// Do nothing...
		}
		else
		{
			tmp |= UART_LCR_PARITY_EN;
			switch (parity)
			{
			case UART_PARITY_ODD:
				tmp |= UART_LCR_PARITY_ODD;
				break;

			case UART_PARITY_EVEN:
				tmp |= UART_LCR_PARITY_EVEN;
				break;

			case UART_PARITY_SP_1:
				tmp |= UART_LCR_PARITY_F_1;
				break;

			case UART_PARITY_SP_0:
				tmp |= UART_LCR_PARITY_F_0;
				break;
			default:
				break;
			}
		}

		switch (stopBits){
		case UART_STOPBIT_2:
			tmp |= UART_LCR_STOPBIT_SEL;
			break;
		case UART_STOPBIT_1:
		default:
			// Do no thing
			break;
		}

		// Write back to LCR
		if (((LPC_UART1_TypeDef *)UARTx) ==  LPC_UART1)
		{
			((LPC_UART1_TypeDef *)UARTx)->LCR = (uint8_t)(tmp & UART_LCR_BITMASK);
		}
		else
		{
			UARTx->LCR = (uint8_t)(tmp & UART_LCR_BITMASK);
		}
	}

	static bool txEmpty() {
		return UARTx->LSR & UART_LSR_THRE;
	}

	static bool rxEmpty() {
		return !(UARTx->LSR & UART_LSR_RDR);
	}

	static void write(char c) {

		if(fifoLevel == UART_TX_FIFO_SIZE) {
			// Wait for THR empty with timeout
			while (!(UARTx->LSR & UART_LSR_THRE));
			fifoLevel = 0;
		}

		UARTx->THR = c;

		fifoLevel++;
	}

	static bool read(uint8_t& c) {
		if(UARTx->LCR & UART_LSR_RDR) {
			c = UARTx->RBR;
			return true;
		}
		return false;
	}

	static ALWAYS_INLINE
	void put(char c) {
		UARTx->THR = c;
	}

	static ALWAYS_INLINE
	char get() {
		return UARTx->RBR;
	}

	static void enableTxCompleteInterrupt(bool en) {
		if(en)
			UARTx->IER |= UART_IER_THREINT_EN;
		else
			UARTx->IER &= ~UART_IER_THREINT_EN;
	}

	static void enableRxCompleteInterrupt(bool en) {
		if(en)
			UARTx->IER |= UART_IER_RBRINT_EN;
		else
			UARTx->IER &= ~UART_IER_RBRINT_EN;
	}


	static void attachTxCompleteInterrupt(void (*f)()) {
		txCallback = f;
	}

	static void attachRxCompleteInterrupt(void (*f)()) {
		rxCallback = f;
	}

	static void handleIRQ() {
		uint32_t intstat = LPC_UART0->IIR;
		if(!(intstat & 1)) {
			switch(intstat & 0xF) {
			case UART_IIR_INTID_THRE:
				if(txCallback != 0)
					txCallback();
				break;
			case UART_IIR_INTID_RDA:
			case UART_IIR_INTID_CTI:
				if(rxCallback != 0)
					rxCallback();
				else
					UARTx->RBR;
				break;
			}
		}
	}

private:
	static void (*txCallback)();
	static void (*rxCallback)();

	static bool uart_set_divisors(uint32_t baudrate)
	{
		bool returnStatus = false;

		uint32_t uClk;
		uint32_t calcBaudrate = 0;
		uint32_t temp = 0;

		uint32_t mulFracDiv, dividerAddFracDiv;
		uint32_t diviser = 0 ;
		uint32_t mulFracDivOptimal = 1;
		uint32_t dividerAddOptimal = 0;
		uint32_t diviserOptimal = 0;

		uint32_t relativeError = 0;
		uint32_t relativeOptimalError = 100000;

		/* get UART block clock */
		if (UARTx == LPC_UART0)
		{
			uClk = CLKPwr::getPCLK(CLKPwr::ClkType::UART0);
		}
		else if (UARTx == (LPC_UART_TypeDef *)LPC_UART1)
		{
			uClk = CLKPwr::getPCLK(CLKPwr::ClkType::UART1);
		}
		else if (UARTx == LPC_UART2)
		{
			uClk = CLKPwr::getPCLK(CLKPwr::ClkType::UART2);
		}
		else if (UARTx == LPC_UART3)
		{
			uClk = CLKPwr::getPCLK(CLKPwr::ClkType::UART3);
		}


		uClk = uClk >> 4; /* div by 16 */
		/* In the Uart IP block, baud rate is calculated using FDR and DLL-DLM registers
		* The formula is :
		* BaudRate= uClk * (mulFracDiv/(mulFracDiv+dividerAddFracDiv) / (16 * (DLL)
		* It involves floating point calculations. That's the reason the formulae are adjusted with
		* Multiply and divide method.*/
		/* The value of mulFracDiv and dividerAddFracDiv should comply to the following expressions:
		* 0 < mulFracDiv <= 15, 0 <= dividerAddFracDiv <= 15 */
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
			if (((LPC_UART1_TypeDef *)UARTx) == LPC_UART1)
			{
				((LPC_UART1_TypeDef *)UARTx)->LCR |= UART_LCR_DLAB_EN;
				((LPC_UART1_TypeDef *)UARTx)->/*DLIER.*/DLM = UART_LOAD_DLM(diviserOptimal);
				((LPC_UART1_TypeDef *)UARTx)->/*RBTHDLR.*/DLL = UART_LOAD_DLL(diviserOptimal);
				/* Then reset DLAB bit */
				((LPC_UART1_TypeDef *)UARTx)->LCR &= (~UART_LCR_DLAB_EN) & UART_LCR_BITMASK;
				((LPC_UART1_TypeDef *)UARTx)->FDR = (UART_FDR_MULVAL(mulFracDivOptimal) \
						| UART_FDR_DIVADDVAL(dividerAddOptimal)) & UART_FDR_BITMASK;
			}
			else
			{
				UARTx->LCR |= UART_LCR_DLAB_EN;
				UARTx->/*DLIER.*/DLM = UART_LOAD_DLM(diviserOptimal);
				UARTx->/*RBTHDLR.*/DLL = UART_LOAD_DLL(diviserOptimal);
				/* Then reset DLAB bit */
				UARTx->LCR &= (~UART_LCR_DLAB_EN) & UART_LCR_BITMASK;
				UARTx->FDR = (UART_FDR_MULVAL(mulFracDivOptimal) \
						| UART_FDR_DIVADDVAL(dividerAddOptimal)) & UART_FDR_BITMASK;
			}
			returnStatus = true;
		}

		return returnStatus;
	}

	static uint8_t fifoLevel;
};

template<int P_UART>
uint8_t Uart<P_UART>::fifoLevel = 0;

typedef Uart<(int)LPC_UART0> Uart0;
typedef Uart<(int)LPC_UART1> Uart1;
typedef Uart<(int)LPC_UART2> Uart2;
typedef Uart<(int)LPC_UART3> Uart3;

#undef UARTx
}
}
