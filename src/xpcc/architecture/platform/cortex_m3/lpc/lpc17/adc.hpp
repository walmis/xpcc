/*
 * adc.hpp
 *
 *  Created on: Oct 10, 2013
 *      Author: walmis
 */

#ifndef ADC_HPP_
#define ADC_HPP_

#include <xpcc/architecture.hpp>

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
	static void init(int rate = 200000);
	static void deinit();
	static void start(ADCStartMode start_mode);

	static uint32_t getData(uint32_t channel);

	/***
	 * The AD converter does repeated conversions at up to 200 kHz, scanning (if necessary)
	 through the pins selected by bits set to ones in the SEL field. The first conversion after the
	 start corresponds to the least-significant 1 in the SEL field, then higher numbered 1-bits
	 (pins) if applicable. Repeated conversions can be terminated by clearing this bit, but the
	 conversion that’s in progress when this bit is cleared will be completed.
	 Remark: START bits must be 000 when BURST = 1 or conversions will not start. If
	 BURST is set to 1, the ADGINTEN bit in the AD0INTEN register (Table 534) must be set
	 to 0.
	 */
	static void burstMode(bool enable = true);
	static void enableChannel(uint8_t channel, bool enable = true);
	static void enableChannelInt(uint8_t channel, bool enable = true);
	static void powerDown(bool powerdown = true);
	static void edgeStartConfig(StartEdge EdgeOption);
	static void enableGlobalInt(bool enable);
	static uint32_t getData();
	static bool isDone(uint8_t channel);
	static bool isOverrun(uint8_t channel);
	static bool isDone();
	static bool isOverrun();
};


}
}
/**@}*/
#endif /* ADC_HPP_ */
