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
/**
 * @brief		General Purpose Timer x
 * 
 * Interrupt handler:
 * \code
 * extern "C" void
 * TIMx_IRQHandler(void)
 * {
 *     Timerx::resetInterruptFlags(Timerx::...);
 *     
 *     ...
 * }
 * \endcode
 * 
 * \warning	The Timer has much more possibilities than presented by this
 * 			interface (e.g. Input Capture, Trigger for other Timers, DMA).
 * 			It might be expanded in the future.
 * 
 * @author		Fabian Greif
 * @ingroup		stm32
 */

TIM_TypeDef* const gp_timers[] = { 0, TIM2, TIM3, TIM4, TIM5, 0, 0, 0, TIM9,
		TIM10, TIM11, TIM12, TIM13, TIM14 };


#define TIMx (gp_timers[timid-1])

template <int timid>
class GPTimer : public GeneralPurposeTimer
{
public:

public:

	enum class MasterMode : uint32_t
	{
		Reset 			= 0,							// 0b000
		Enable 			= TIM_CR2_MMS_0,				// 0b001
		Update 			= TIM_CR2_MMS_1,				// 0b010
		Pulse 			= TIM_CR2_MMS_1 | TIM_CR2_MMS_0,// 0b011
		CompareOc1Ref 	= TIM_CR2_MMS_2,				// 0b100
		CompareOc2Ref 	= TIM_CR2_MMS_2 | TIM_CR2_MMS_0,// 0b101
	//%% if id >= 2 and id < 5
		// Only available on TIM2/3/4
		CompareOc3Ref 	= TIM_CR2_MMS_2 | TIM_CR2_MMS_1,// 0b110
		CompareOc4Ref 	= TIM_CR2_MMS_2 | TIM_CR2_MMS_1	// 0b111
										| TIM_CR2_MMS_0,
	//%% endif
	};

	enum class SlaveModeTrigger : uint32_t
	{
	//%% if id < 2 or id > 5
		Internal0 = 0,
	//%% endif
		Internal1 = TIM_SMCR_TS_0,
		Internal2 = TIM_SMCR_TS_1,
	//%% if (id < 2 or id > 5) and id != 9 and id != 12
		Internal3 = TIM_SMCR_TS_1 | TIM_SMCR_TS_0,
	//%% endif
		TimerInput1EdgeDetector = TIM_SMCR_TS_2,
		TimerInput1Filtered = TIM_SMCR_TS_2 | TIM_SMCR_TS_0,
		TimerInput2Filtered = TIM_SMCR_TS_2 | TIM_SMCR_TS_1,
	//%% if id != 9 and id != 12
		External = TIM_SMCR_TS_2 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0,
	//%% endif
	};

	enum class SlaveMode : uint32_t
	{
		/// Slave mode disabled - if CEN = '1' then the prescaler is clocked directly by the internal clock.
		Disabled	= 0,
	///%% if id != 9 and id != 12
		/// Counter counts up/down on TI2FP2 edge depending on TI1FP1 level.
		Encoder1	= TIM_SMCR_SMS_0,
		/// Counter counts up/down on TI1FP1 edge depending on TI2FP2 level.
		Encoder2	= TIM_SMCR_SMS_1,
		/// Counter counts up/down on both TI1FP1 and TI2FP2 edges depending on the level of the other input.
		Encoder3	= TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0,
	//%% endif
		/// Rising edge of the selected trigger input (TRGI) reinitializes the counter and generates an update of the registers.
		Reset		= TIM_SMCR_SMS_2,
		/// The counter clock is enabled when the trigger input (TRGI) is high. The counter stops (but is not reset) as soon as the trigger becomes low. Both start and stop of the counter are controlled.
		Gated		= TIM_SMCR_SMS_2 | TIM_SMCR_SMS_0,
		/// The counter starts at a rising edge of the trigger TRGI (but it is not reset). Only the start of the counter is controlled.
		Trigger	= TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1,
		/// Rising edges of the selected trigger (TRGI) clock the counter.
		ExternalClock = TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0,
	//%% if target is stm32f3 and (id >= 2 and id <= 4)
		/// reinitialize and start counter
	//	SLAVE_RESET_TRIGGER	= TIM_SMCR_SMS_3,
	//%% endif
	};

	// This type is the internal size of the counter.
	// Timer 2 and 5 are the only one which have the size of 32 bit and
	// only on st32f2 and st32f4
//%% if id in [2, 5] and (target is stm32f2 or target is stm32f3 or target is stm32f4)
	typedef uint32_t Value;
//%% else
	//typedef uint16_t Value;
//%% endif

	static inline void
	pause()
	{
		TIMx->CR1 &= ~TIM_CR1_CEN;
	}

	static inline void
	start()
	{
		TIMx->CR1 |= TIM_CR1_CEN;
	}

	static inline void
	setPrescaler(uint16_t prescaler)
	{
		// Because a prescaler of zero is not possible the actual
		// prescaler value is \p prescaler - 1 (see Datasheet)
		TIMx->PSC = prescaler - 1;
	}

	static inline void
	setOverflow(Value overflow)
	{
		TIMx->ARR = overflow;
	}

	static Value
	setPeriod(uint32_t microseconds, bool autoApply = true)
	{
		// This will be inaccurate for non-smooth frequencies (last six digits
		// unequal to zero)
		uint32_t freq;
		if(timid == 9 || timid == 10 || timid == 11) {
			freq = Clocks::getPCLK2Frequency()*2;
		} else {
			freq = Clocks::getPCLK1Frequency()*2;
		}

		uint32_t cycles = microseconds * (freq / 1000000UL);
		uint16_t prescaler = (cycles + 65535) / 65536;	// always round up
		Value overflow = cycles / prescaler;

		overflow = overflow - 1;	// e.g. 36000 cycles are from 0 to 35999

		setPrescaler(prescaler);
		setOverflow(overflow);

		if (autoApply) {
			// Generate Update Event to apply the new settings for ARR
			TIMx->EGR |= TIM_EGR_UG;
		}

		return overflow;
	}

	static inline void
	applyAndReset()
	{
		// Generate Update Event to apply the new settings for ARR
		TIMx->EGR |= TIM_EGR_UG;
	}

	static inline Value
	getValue()
	{
		return TIMx->CNT;
	}

	static inline void
	setValue(Value value)
	{
		TIMx->CNT = value;
	}

public:

	static inline void
	setCompareValue(uint32_t channel, Value value)
	{
#if defined(STM32F2XX) || defined(STM32F3XX) || defined(STM32F4XX)
		*(&TIMx->CCR1 + (channel - 1)) = value;
#else
		*(&TIMx->CCR1 + ((channel - 1) * 2)) = value;
#endif
	}

	static inline Value
	getCompareValue(uint32_t channel)
	{
#if defined(STM32F2XX) || defined(STM32F3XX) || defined(STM32F4XX)
		return *(&TIMx->CCR1 + (channel - 1));
#else
		return *(&TIMx->CCR1 + ((channel - 1) * 2));
#endif
	}

public:
	static void
	enableInterruptVector(bool enable, uint32_t priority);

	static inline void
	enableInterrupt(Interrupt interrupt)
	{
		TIMx->DIER |= static_cast<uint32_t>(interrupt);
	}

	static inline void
	disableInterrupt(Interrupt interrupt)
	{
		TIMx->DIER &= ~static_cast<uint32_t>(interrupt);
	}

	static inline void
	enableDmaRequest(DmaRequestEnable dmaRequests)
	{
		TIMx->DIER |= static_cast<uint32_t>(dmaRequests);
	}

	static inline void
	disableDmaRequest(DmaRequestEnable dmaRequests)
	{
		TIMx->DIER &= ~static_cast<uint32_t>(dmaRequests);
	}

	static inline InterruptFlag
	getInterruptFlags()
	{
		return (InterruptFlag) TIMx->SR;
	}

	static inline void
	acknowledgeInterruptFlags(InterruptFlag flags)
	{
		// Flags are cleared by writing a zero to the flag position.
		// Writing a one is ignored.
		TIMx->SR = ~static_cast<uint32_t>(flags);
	}
//
	static void
	enable();

	static void
	disable();

	// ----------------------------------------------------------------------------
	static void
	setMode(Mode mode, SlaveMode slaveMode = SlaveMode::Disabled,
			SlaveModeTrigger slaveModeTrigger = static_cast<SlaveModeTrigger>(0),
			MasterMode masterMode = MasterMode::Reset,
			bool enableOnePulseMode = false)
	{
		// disable timer
		TIMx->CR1 = 0;
		TIMx->CR2 = 0;

		if (slaveMode == SlaveMode::Encoder1 || \
			slaveMode == SlaveMode::Encoder2 || \
			slaveMode == SlaveMode::Encoder3)
		{
			// Prescaler has to be 1 when using the quadrature decoder
			setPrescaler(1);
		}

		// ARR Register is buffered, only Under/Overflow generates update interrupt
		if(enableOnePulseMode) {
			TIMx->CR1 = TIM_CR1_ARPE | TIM_CR1_URS | TIM_CR1_OPM
											| static_cast<uint32_t>(mode);
		} else {
			TIMx->CR1 = TIM_CR1_ARPE | TIM_CR1_URS
											| static_cast<uint32_t>(mode);
		}
		TIMx->CR2 = static_cast<uint32_t>(masterMode);
		TIMx->SMCR = static_cast<uint32_t>(slaveMode)
							| static_cast<uint32_t>(slaveModeTrigger);
	}

	// ----------------------------------------------------------------------------
	static void
	configureInputChannel(uint32_t channel, InputCaptureMapping input,
			InputCapturePrescaler prescaler,
			InputCapturePolarity polarity, uint8_t filter,
			bool xor_ch1_3=false)
	{
		channel -= 1;	// 1..4 -> 0..3

		// disable channel
		TIMx->CCER &= ~((TIM_CCER_CC1NP | TIM_CCER_CC1P | TIM_CCER_CC1E) << (channel * 4));

		uint32_t flags = static_cast<uint32_t>(input);
		flags |= static_cast<uint32_t>(prescaler) << 2;
		flags |= (static_cast<uint32_t>(filter) & 0xf) << 4;

		if (channel <= 1)
		{
			uint32_t offset = 8 * channel;

			flags <<= offset;
			flags |= TIMx->CCMR1 & ~(0xff << offset);

			TIMx->CCMR1 = flags;

			if(channel == 0) {
				if(xor_ch1_3)
					TIMx->CR2 |= TIM_CR2_TI1S;
				else
					TIMx->CR2 &= ~TIM_CR2_TI1S;
			}
		}
		else {
			uint32_t offset = 8 * (channel - 2);

			flags <<= offset;
			flags |= TIMx->CCMR2 & ~(0xff << offset);

			TIMx->CCMR2 = flags;
		}

		TIMx->CCER |=
			(TIM_CCER_CC1E | static_cast<uint32_t>(polarity)) << (channel * 4);
	}

	// ----------------------------------------------------------------------------
	static void
	configureOutputChannel(uint32_t channel, OutputCompareMode mode,
			Value compareValue, PinState out = PinState::Enable)
	{
		channel -= 1;	// 1..4 -> 0..3

		// disable channel
		TIMx->CCER &= ~((TIM_CCER_CC1NP | TIM_CCER_CC1P | TIM_CCER_CC1E) << (channel * 4));

		setCompareValue(channel + 1, compareValue);

		// enable preload (the compare value is loaded at each update event)
		uint32_t flags = static_cast<uint32_t>(mode) | TIM_CCMR1_OC1PE;

		if (channel <= 1)
		{
			uint32_t offset = 8 * channel;

			flags <<= offset;
			flags |= TIMx->CCMR1 & ~(0xff << offset);

			TIMx->CCMR1 = flags;
		}
		else {
			uint32_t offset = 8 * (channel - 2);

			flags <<= offset;
			flags |= TIMx->CCMR2 & ~(0xff << offset);

			TIMx->CCMR2 = flags;
		}

		if (mode != OutputCompareMode::Inactive && out == PinState::Enable) {
			TIMx->CCER |= (TIM_CCER_CC1E) << (channel * 4);
		}
	}

	// ----------------------------------------------------------------------------
//	{
//		if (enable) {
//			// Set priority for the interrupt vector
//			//NVIC_SetPriority(TIMx_IRQn, priority);
//
//			// register IRQ at the NVIC
//			switch(timid+1) {
//			case 2:
//				NVIC_EnableIRQ(TIM2_IRQn);
//				return;
//			case 3:
//				NVIC_EnableIRQ(TIM3_IRQn);
//				return;
//			case 4:
//				NVIC_EnableIRQ(TIM4_IRQn);
//				return;
//			case 5:
//				NVIC_EnableIRQ(TIM5_IRQn);
//				return;
//			case 9:
//				NVIC_EnableIRQ(TIM9_IRQn);
//				return;
//			case 10:
//				NVIC_EnableIRQ(TIM10_IRQn);
//				return;
//			case 11:
//				NVIC_EnableIRQ(TIM11_IRQn);
//				return;
//			case 12:
//				NVIC_EnableIRQ(TIM12_IRQn);
//				return;
//			case 13:
//				NVIC_EnableIRQ(TIM13_IRQn);
//				return;
//			case 14:
//				NVIC_EnableIRQ(TIM14_IRQn);
//				return;
//			}
//
//		}
//		else {
//			//NVIC_DisableIRQ(TIMx_IRQn);
//		}
//	}

};

typedef GPTimer<2> GPTimer2;
typedef GPTimer<3> GPTimer3;
typedef GPTimer<4> GPTimer4;
typedef GPTimer<5> GPTimer5;
typedef GPTimer<9> GPTimer9;
typedef GPTimer<10> GPTimer10;
typedef GPTimer<11> GPTimer11;
typedef GPTimer<12> GPTimer12;
typedef GPTimer<13> GPTimer13;
typedef GPTimer<14> GPTimer14;

}
}

#undef TIMx
