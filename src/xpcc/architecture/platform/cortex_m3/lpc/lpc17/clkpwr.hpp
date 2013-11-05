/*
 * clkpwr.hpp
 *
 *  Created on: Oct 9, 2013
 *      Author: walmis
 */

#ifndef CLKPWR_HPP_
#define CLKPWR_HPP_


#include <xpcc/architecture.hpp>

#define CLKPWR_PCLKSEL_GET(p, n)	((uint32_t)((n>>p)&0x03))
#define CLKPWR_PCLKSEL_SET(p,n)		(n<<p)
/** Macro to mask peripheral clock of each type */
#define CLKPWR_PCLKSEL_BITMASK(p)	(0x03<<p)

/** @ingroup lpc17
 *  @defgroup clkpwr LPC17xx Clocking and power
 */

/** @ingroup clkpwr
 */
class CLKPwr {
public:
	enum ClkType {
		/**********************************************************************
		 * Peripheral Clock Selection Definitions
		 **********************************************************************/
		/** Peripheral clock divider bit position for WDT */
		WDT = ((uint32_t) (0)),
		/** Peripheral clock divider bit position for TIMER0 */
		TIMER0 = ((uint32_t) (2)),
		/** Peripheral clock divider bit position for TIMER1 */
		TIMER1 = ((uint32_t) (4)),
		/** Peripheral clock divider bit position for UART0 */
		UART0 = ((uint32_t) (6)),
		/** Peripheral clock divider bit position for UART1 */
		UART1 = ((uint32_t) (8)),
		/** Peripheral clock divider bit position for PWM1 */
		PWM1 = ((uint32_t) (12)),
		/** Peripheral clock divider bit position for I2C0 */
		I2C0 = ((uint32_t) (14)),
		/** Peripheral clock divider bit position for SPI */
		SPI = ((uint32_t) (16)),
		/** Peripheral clock divider bit position for SSP1 */
		SSP1 = ((uint32_t) (20)),
		/** Peripheral clock divider bit position for DAC */
		DAC = ((uint32_t) (22)),
		/** Peripheral clock divider bit position for ADC */
		ADC = ((uint32_t) (24)),
		/** Peripheral clock divider bit position for CAN1 */
		CAN1 = ((uint32_t) (26)),
		/** Peripheral clock divider bit position for CAN2 */
		CAN2 = ((uint32_t) (28)),
		/** Peripheral clock divider bit position for ACF */
		ACF = ((uint32_t) (30)),
		/** Peripheral clock divider bit position for QEI */
		QEI = ((uint32_t) (32)),
		/** Peripheral clock divider bit position for PCB */
		PCB = ((uint32_t) (36)),
		/** Peripheral clock divider bit position for  I2C1 */
		I2C1 = ((uint32_t) (38)),
		/** Peripheral clock divider bit position for SSP0 */
		SSP0 = ((uint32_t) (42)),
		/** Peripheral clock divider bit position for TIMER2 */
		TIMER2 = ((uint32_t) (44)),
		/** Peripheral clock divider bit position for  TIMER3 */
		TIMER3 = ((uint32_t) (46)),
		/** Peripheral clock divider bit position for UART2 */
		UART2 = ((uint32_t) (48)),
		/** Peripheral clock divider bit position for UART3 */
		UART3 = ((uint32_t) (50)),
		/** Peripheral clock divider bit position for I2C2 */
		I2C2 = ((uint32_t) (52)),
		/** Peripheral clock divider bit position for I2S */
		I2S = ((uint32_t) (54)),
		/** Peripheral clock divider bit position for RIT */
		RIT = ((uint32_t) (58)),
		/** Peripheral clock divider bit position for SYSCON */
		SYSCON = ((uint32_t) (60)),
		/** Peripheral clock divider bit position for MC */
		MC = ((uint32_t) (62))
	};

	enum PType {
		/********************************************************************
		 * Power Control for Peripherals Definitions
		 **********************************************************************/
		/** Timer/Counter 0 power/clock control bit */
		PCTIM0 = (uint32_t) (1 << 1),
		/* Timer/Counter 1 power/clock control bit */
		PCTIM1 = (uint32_t) (1 << 2),
		/** UART0 power/clock control bit */
		PCUART0 = (uint32_t) (1 << 3),
		/** UART1 power/clock control bit */
		PCUART1 = (uint32_t) (1 << 4),
		/** PWM1 power/clock control bit */
		PCPWM1 = (uint32_t) (1 << 6),
		/** The I2C0 interface power/clock control bit */
		PCI2C0 = (uint32_t) (1 << 7),
		/** The SPI interface power/clock control bit */
		PCSPI = (uint32_t) (1 << 8),
		/** The RTC power/clock control bit */
		PCRTC = (uint32_t) (1 << 9),
		/** The SSP1 interface power/clock control bit */
		PCSSP1 = (uint32_t) (1 << 10),
		/** A/D converter 0 (ADC0) power/clock control bit */
		PCAD = (uint32_t) (1 << 12),
		/** CAN Controller 1 power/clock control bit */
		PCAN1 = (uint32_t) (1 << 13),
		/** CAN Controller 2 power/clock control bit */
		PCAN2 = (uint32_t) (1 << 14),
		/** GPIO power/clock control bit */
		PCGPIO = (uint32_t) (1 << 15),
		/** Repetitive Interrupt Timer power/clock control bit */
		PCRIT = (uint32_t) (1 << 16),
		/** Motor Control PWM */
		PCMC = (uint32_t) (1 << 17),
		/** Quadrature Encoder Interface power/clock control bit */
		PCQEI = (uint32_t) (1 << 18),
		/** The I2C1 interface power/clock control bit */
		PCI2C1 = (uint32_t) (1 << 19),
		/** The SSP0 interface power/clock control bit */
		PCSSP0 = (uint32_t) (1 << 21),
		/** Timer 2 power/clock control bit */
		PCTIM2 = (uint32_t) (1 << 22),
		/** Timer 3 power/clock control bit */
		PCTIM3 = (uint32_t) (1 << 23),
		/** UART 2 power/clock control bit */
		PCUART2 = (uint32_t) (1 << 24),
		/** UART 3 power/clock control bit */
		PCUART3 = (uint32_t) (1 << 25),
		/** I2C interface 2 power/clock control bit */
		PCI2C2 = (uint32_t) (1 << 26),
		/** I2S interface power/clock control bit*/
		PCI2S = (uint32_t) (1 << 27),
		/** GP DMA function power/clock control bit*/
		PCGPDMA = (uint32_t) (1 << 29),
		/** Ethernet block power/clock control bit*/
		PCENET = (uint32_t) (1 << 30),
		/** USB interface power/clock control bit*/
		PCUSB = (uint32_t) (1 << 31)
	};

	enum ClkDiv {
		/** Peripheral clock divider is set to 4 from CCLK. This is the default in LPC17xx */
		DIV_4  = ((uint32_t)(0)),
		/** Peripheral clock divider is the same with CCLK */
		DIV_1  = ((uint32_t)(1)),
		/** Peripheral clock divider is set to 2 from CCLK */
		DIV_2  = ((uint32_t)(2))
	};

	/// Set peripheral clock power.
	/// @param peripheral type
	/// @param enable or disable peripheral clock
	static void setClkPower(PType type, bool power)
	{
		if (power)
		{
			LPC_SC->PCONP |= type;
		}
		else
		{
			LPC_SC->PCONP &= (~type);
		}
	}
	/// Set peripheral clock divisor value
	static void setClkDiv(ClkType type, ClkDiv div) {
		uint32_t bitpos;

		bitpos = (type < 32) ? (type) : (type - 32);

		/* PCLKSEL0 selected */
		if (type < 32)
		{
			/* Clear two bit at bit position */
			LPC_SC->PCLKSEL0 &= (~(CLKPWR_PCLKSEL_BITMASK(bitpos)));

			/* Set two selected bit */
			LPC_SC->PCLKSEL0 |= (CLKPWR_PCLKSEL_SET(bitpos, div));
		}
		/* PCLKSEL1 selected */
		else
		{
			/* Clear two bit at bit position */
			LPC_SC->PCLKSEL1 &= ~(CLKPWR_PCLKSEL_BITMASK(bitpos));

			/* Set two selected bit */
			LPC_SC->PCLKSEL1 |= (CLKPWR_PCLKSEL_SET(bitpos, div));
		}
	}

	/// Get peripheral clock divisor value
	static ClkDiv getPCLKSEL (ClkType ClkType)
	{
		uint32_t bitpos, retval;

		if (ClkType < 32)
		{
			bitpos = ClkType;
			retval = LPC_SC->PCLKSEL0;
		}
		else
		{
			bitpos = ClkType - 32;
			retval = LPC_SC->PCLKSEL1;
		}

		retval = CLKPWR_PCLKSEL_GET(bitpos, retval);
		return (ClkDiv)retval;
	}

	/// Get peripheral clock speed defined by ClkType
	static uint32_t getPCLK (ClkType ClkType)
	{
		uint32_t retval, div;

		retval = SystemCoreClock;
		div = getPCLKSEL(ClkType);

		switch (div)
		{
		case 0:
			div = 4;
			break;

		case 1:
			div = 1;
			break;

		case 2:
			div = 2;
			break;

		case 3:
			div = 8;
			break;
		}
		retval /= div;

		return retval;
	}

//TODO: Port other functions

};

#endif /* CLKPWR_HPP_ */
