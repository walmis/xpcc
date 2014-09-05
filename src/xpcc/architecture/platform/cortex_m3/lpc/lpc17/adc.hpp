/*
 * adc.hpp
 *
 *  Created on: Oct 10, 2013
 *      Author: walmis
 */

#ifndef ADC_HPP_
#define ADC_HPP_

#include <xpcc/architecture.hpp>

/** @ingroup lpc17
 * @defgroup ADC LPC17xx ADC Controller
 *
 *
 */

/**@{*/
/**  Selects which of the AD0.0:7 pins is (are) to be sampled and converted */
#define ADC_CR_CH_SEL(n)	((1UL << n))
/**  The APB clock (PCLK) is divided by (this value plus one)
* to produce the clock for the A/D */
#define ADC_CR_CLKDIV(n)	((n<<8))
/**  Repeated conversions A/D enable bit */
#define ADC_CR_BURST		((1UL<<16))
/**  ADC convert in power down mode */
#define ADC_CR_PDN			((1UL<<21))
/**  Start mask bits */
#define ADC_CR_START_MASK	((7UL<<24))
/**  Select Start Mode */
#define ADC_CR_START_MODE_SEL(SEL)	((SEL<<24))
/**  Start conversion now */
#define ADC_CR_START_NOW	((1UL<<24))
/**  Start conversion when the edge selected by bit 27 occurs on P2.10/EINT0 */
#define ADC_CR_START_EINT0	((2UL<<24))
/** Start conversion when the edge selected by bit 27 occurs on P1.27/CAP0.1 */
#define ADC_CR_START_CAP01	((3UL<<24))
/**  Start conversion when the edge selected by bit 27 occurs on MAT0.1 */
#define ADC_CR_START_MAT01	((4UL<<24))
/**  Start conversion when the edge selected by bit 27 occurs on MAT0.3 */
#define ADC_CR_START_MAT03	((5UL<<24))
/**  Start conversion when the edge selected by bit 27 occurs on MAT1.0 */
#define ADC_CR_START_MAT10	((6UL<<24))
/**  Start conversion when the edge selected by bit 27 occurs on MAT1.1 */
#define ADC_CR_START_MAT11	((7UL<<24))
/**  Start conversion on a falling edge on the selected CAP/MAT signal */
#define ADC_CR_EDGE			((1UL<<27))

/*********************************************************************//**
 * Macro defines for ADC Global Data register
 **********************************************************************/
/** When DONE is 1, this field contains result value of ADC conversion */
#define ADC_GDR_RESULT(n)		(((n>>4)&0xFFF))
/** These bits contain the channel from which the LS bits were converted */
#define ADC_GDR_CH(n)			(((n>>24)&0x7))
/** This bit is 1 in burst mode if the results of one or
 * more conversions was (were) lost */
#define ADC_GDR_OVERRUN_FLAG	((1UL<<30))
/** This bit is set to 1 when an A/D conversion completes */
#define ADC_GDR_DONE_FLAG		((1UL<<31))

/** This bits is used to mask for Channel */
#define ADC_GDR_CH_MASK		((7UL<<24))
/*********************************************************************//**
 * Macro defines for ADC Interrupt register
 **********************************************************************/
/** These bits allow control over which A/D channels generate
 * interrupts for conversion completion */
#define ADC_INTEN_CH(n)			((1UL<<n))
/** When 1, enables the global DONE flag in ADDR to generate an interrupt */
#define ADC_INTEN_GLOBAL		((1UL<<8))

/*********************************************************************//**
 * Macro defines for ADC Data register
 **********************************************************************/
/** When DONE is 1, this field contains result value of ADC conversion */
#define ADC_DR_RESULT(n)		(((n>>4)&0xFFF))
/** These bits mirror the OVERRRUN status flags that appear in the
 * result register for each A/D channel */
#define ADC_DR_OVERRUN_FLAG		((1UL<<30))
/** This bit is set to 1 when an A/D conversion completes. It is cleared
 * when this register is read */
#define ADC_DR_DONE_FLAG		((1UL<<31))

/*********************************************************************//**
 * Macro defines for ADC Status register
**********************************************************************/
/** These bits mirror the DONE status flags that appear in the result
 * register for each A/D channel */
#define ADC_STAT_CH_DONE_FLAG(n)		((n&0xFF))
/** These bits mirror the OVERRRUN status flags that appear in the
 * result register for each A/D channel */
#define ADC_STAT_CH_OVERRUN_FLAG(n)		(((n>>8)&0xFF))
/** This bit is the A/D interrupt flag */
#define ADC_STAT_INT_FLAG				((1UL<<16))

/*********************************************************************//**
 * Macro defines for ADC Trim register
**********************************************************************/
/** Offset trim bits for ADC operation */
#define ADC_ADCOFFS(n)		(((n&0xF)<<4))
/** Written to boot code*/
#define ADC_TRIM(n)		    (((n&0xF)<<8))

namespace xpcc {
namespace lpc17 {


/** @brief LPC17xx ADC Peripheral */
class ADC {
public:

	enum ADCStartMode
	{
		START_CONTINUOUS =0,	/*!< Continuous mode */
		START_NOW,				/*!< Start conversion now */
		START_ON_EINT0,			/*!< Start conversion when the edge selected
									 * by bit 27 occurs on P2.10/EINT0 */
		START_ON_CAP01,			/*!< Start conversion when the edge selected
									 * by bit 27 occurs on P1.27/CAP0.1 */
		START_ON_MAT01,			/*!< Start conversion when the edge selected
									 * by bit 27 occurs on MAT0.1 */
		START_ON_MAT03,			/*!< Start conversion when the edge selected
									 * by bit 27 occurs on MAT0.3 */
		START_ON_MAT10,			/*!< Start conversion when the edge selected
									  * by bit 27 occurs on MAT1.0 */
		START_ON_MAT11			/*!< Start conversion when the edge selected
									  * by bit 27 occurs on MAT1.1 */
	};

	enum StartEdge {
		RISING = 0, /*!< Start on Rising edge */
		FALLING = 1 /*!< Start on Falling edge */
	};

	//rate - ADC conversion rate in Hz
	static void init(int rate = 200000) {
		uint32_t temp, tmp;

		// Turn on power and clock
		CLKPwr::setClkPower(CLKPwr::PType::PCAD, true);

		LPC_ADC->ADCR = 0;

		//Enable PDN bit
		tmp = ADC_CR_PDN;
		// Set clock frequency
		temp = CLKPwr::getPCLK(CLKPwr::ClkType::ADC);
		/* The APB clock (PCLK_ADC0) is divided by (CLKDIV+1) to produce the clock for
		 * A/D converter, which should be less than or equal to 13MHz.
		 * A fully conversion requires 65 of these clocks.
		 * ADC clock = PCLK_ADC0 / (CLKDIV + 1);
		 * ADC rate = ADC clock / 65;
		 */
		temp = (temp /(rate * 65)) - 1;
		tmp |=  ADC_CR_CLKDIV(temp);

		LPC_ADC->ADCR = tmp;
	}

	static void deinit() {
		// Clear PDN bit
		LPC_ADC->ADCR &= ~ADC_CR_PDN;
		// Turn on power and clock
		CLKPwr::setClkPower(CLKPwr::PType::PCAD, false);
	}

	static uint32_t getData(uint32_t channel)
	{
		uint32_t adc_value;

		adc_value = *(uint32_t *)((&LPC_ADC->ADDR0) + channel);
		return ADC_GDR_RESULT(adc_value);
	}

	static void start(ADCStartMode start_mode)
	{
		LPC_ADC->ADCR &= ~ADC_CR_START_MASK;
		LPC_ADC->ADCR |= ADC_CR_START_MODE_SEL((uint32_t)start_mode);
	}

	/***
	 * The AD converter does repeated conversions at up to 200 kHz, scanning (if necessary)
	 through the pins selected by bits set to ones in the SEL field. The first conversion after the
	 start corresponds to the least-significant 1 in the SEL field, then higher numbered 1-bits
	 (pins) if applicable. Repeated conversions can be terminated by clearing this bit, but the
	 conversion that’s in progress when this bit is cleared will be completed.
	 */
	static void burstMode(bool enable = true)
	{
		LPC_ADC->ADCR &= ~ADC_CR_BURST;
		if (enable){
			LPC_ADC->ADCR |= ADC_CR_BURST;
		}
	}

	static void enableChannel(uint8_t channel, bool enable = true)
	{

		if (enable) {
			LPC_ADC->ADCR |= ADC_CR_CH_SEL(channel);
		} else {
			LPC_ADC->ADCR &= ~ADC_CR_CH_SEL(channel);
		}
	}

	static void enableChannelInt(uint8_t channel, bool enable = true)
	{
		LPC_ADC->ADINTEN &= ~ADC_INTEN_CH(channel);
		if (enable){
			LPC_ADC->ADINTEN |= ADC_INTEN_CH(channel);
		}
	}

	static void powerDown(bool powerdown = true)
	{
		LPC_ADC->ADCR &= ~ADC_CR_PDN;
		if (powerdown){
			LPC_ADC->ADCR |= ADC_CR_PDN;
		}
	}

	static void edgeStartConfig(StartEdge EdgeOption)
	{
		LPC_ADC->ADCR &= ~ADC_CR_EDGE;
		if (EdgeOption){
			LPC_ADC->ADCR |= ADC_CR_EDGE;
		}
	}

	static void enableGlobalInt(bool enable) {
		enableChannelInt(8, enable);
	}

	static uint32_t getData()
	{
		return ((uint32_t)(LPC_ADC->ADGDR));
	}

	static bool isDone(uint8_t channel) {
		return *(uint32_t *) ((&LPC_ADC->ADDR0) + channel) & ADC_DR_DONE_FLAG;
	}

	static bool isOverrun(uint8_t channel) {
		return *(uint32_t *) ((&LPC_ADC->ADDR0) + channel) & ADC_DR_OVERRUN_FLAG;
	}

	static bool isDone() {
		return LPC_ADC->ADGDR & ADC_DR_DONE_FLAG;
	}

	static bool isOverrun() {
		return LPC_ADC->ADGDR & ADC_DR_OVERRUN_FLAG;
	}


};

}
}
/**@}*/
#endif /* ADC_HPP_ */
