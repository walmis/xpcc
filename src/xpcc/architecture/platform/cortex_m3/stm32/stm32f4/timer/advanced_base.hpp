// coding: utf-8
/* Copyright (c) 2013, Roboterclub Aachen e.V.
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#pragma once

#include "general_purpose_base.hpp"

namespace xpcc
{

namespace stm32
{

class AdvancedControlTimer : public GeneralPurposeTimer
{
public:
	/*
	 * Configure output channel 1..4.
	 *
	 * @param	channel			[1..4]
	 * @param	mode			Output compare mode
	 * @param	compareValue	Preloaded output compare value (can be
	 * 							changed later via setCompareValue())
	 */
	// TODO complementary outputs
	//static void
	//configureOutputChannel(uint32_t channel, OutputCompareMode mode,
	//		uint16_t compareValue);

	// TODO Repetition Counter (TIM1_RCR)


	enum class MasterMode : uint32_t
	{
		Reset 			= 0,							// 0b000
		Enable 			= TIM_CR2_MMS_0,				// 0b001
		Update 			= TIM_CR2_MMS_1,				// 0b010
		ComparePulse 	= TIM_CR2_MMS_1 | TIM_CR2_MMS_0,// 0b011
		CompareOc1Ref 	= TIM_CR2_MMS_2,				// 0b100
		CompareOc2Ref 	= TIM_CR2_MMS_2 | TIM_CR2_MMS_0,// 0b101
		CompareOc3Ref 	= TIM_CR2_MMS_2 | TIM_CR2_MMS_1,// 0b110
		CompareOc4Ref 	= TIM_CR2_MMS_2 | TIM_CR2_MMS_1	// 0b111
										| TIM_CR2_MMS_0,
	};

	/// If capture compare control bit preloading is enabled (CCPC=1),
	/// this selects how to update them.
	enum class CaptureCompareControlUpdate : uint32_t
	{
		/// Update by setting COMG bit.
		SetComg = 0,
		/// Update by setting COMG bit or when a rising edge occures on
		/// the trigger input.
		SetComgOrRisingTrigEdge = TIM_CR2_CCUS
	};

#if defined(STM32F3XX)
	enum class MasterMode2 : uint32_t
	{
		Reset			= 0,							//0b0000
		Enable			= TIM_CR2_MMS2_0,				//0b0001
		Update			= TIM_CR2_MMS2_1,				//0b0010
		ComparePulse	= TIM_CR2_MMS2_1				//0b0010
								| TIM_CR2_MMS2_0,
		CompareOc1Ref	= TIM_CR2_MMS2_2,				//0b0100
		CompareOc2Ref	= TIM_CR2_MMS2_2				//0b0101
								| TIM_CR2_MMS2_0,
		CompareOc3Ref	= TIM_CR2_MMS2_2				//0b0110
								| TIM_CR2_MMS2_1,
		CompareOc4Ref	= TIM_CR2_MMS2_2				//0b0111
								| TIM_CR2_MMS2_1 | TIM_CR2_MMS2_0,
		CompareOc5RefF	= TIM_CR2_MMS2_3,				//0b1000
		CompareOc6Ref	= TIM_CR2_MMS2_3				//0b1001
								| TIM_CR2_MMS2_0,
		// TODO: Add other Master Modes
	};
#endif

	enum class SlaveModeTrigger : uint32_t
	{
		Internal0 = 0,
		Internal1 = TIM_SMCR_TS_0,
		Internal2 = TIM_SMCR_TS_1,
		Internal3 = TIM_SMCR_TS_1 | TIM_SMCR_TS_0,
		TimerInput1EdgeDetector = TIM_SMCR_TS_2,
		TimerInput1Filtered = TIM_SMCR_TS_2 | TIM_SMCR_TS_0,
		TimerInput2Filtered = TIM_SMCR_TS_2 | TIM_SMCR_TS_1,
		External = TIM_SMCR_TS_2 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0,
	};

	enum class SlaveMode : uint32_t
	{
		/// Slave mode disabled - if CEN = '1' then the prescaler is clocked directly by the internal clock.
		Disabled	= 0,
		/// Counter counts up/down on TI2FP2 edge depending on TI1FP1 level.
		Encoder1	= TIM_SMCR_SMS_0,
		/// Counter counts up/down on TI1FP1 edge depending on TI2FP2 level.
		Encoder2	= TIM_SMCR_SMS_1,
		/// Counter counts up/down on both TI1FP1 and TI2FP2 edges depending on the level of the other input.
		Encoder3	= TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0,
		/// Rising edge of the selected trigger input (TRGI) reinitializes the counter and generates an update of the registers.
		Reset		= TIM_SMCR_SMS_2,
		/// The counter clock is enabled when the trigger input (TRGI) is high. The counter stops (but is not reset) as soon as the trigger becomes low. Both start and stop of the counter are controlled.
		Gated		= TIM_SMCR_SMS_2 | TIM_SMCR_SMS_0,
		/// The counter starts at a rising edge of the trigger TRGI (but it is not reset). Only the start of the counter is controlled.
		Trigger		= TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1,
		/// Rising edges of the selected trigger (TRGI) clock the counter.
		ExternalClock = TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0,
	};

	enum class OffStateForRunMode : uint32_t
	{
		Disable = 0,
		Enable  = TIM_BDTR_OSSR,
	};

	enum class OffStateForIdleMode : uint32_t
	{
		Disable = 0,
		Enable  = TIM_BDTR_OSSI,
	};

	enum class OutputIdleState : uint32_t
	{
		Reset = 0,
		Set   = TIM_CR2_OIS1,
	};

	enum class Event : uint32_t
	{
#if defined(STM32F3XX)
		Break2 						= TIM_EGR_B2G,
#endif
		Break 						= TIM_EGR_BG,
		Trigger 					= TIM_EGR_TG,
		CaptureCompareControlUpdate = TIM_EGR_COMG,
		CaptureCompare4 			= TIM_EGR_CC4G,
		CaptureCompare3 			= TIM_EGR_CC3G,
		CaptureCompare2 			= TIM_EGR_CC2G,
		CaptureCompare1 			= TIM_EGR_CC1G,
		Update 						= TIM_EGR_UG,
	};

};
}
}

