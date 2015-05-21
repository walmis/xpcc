// coding: utf-8
/* Copyright (c) 2013, Roboterclub Aachen e.V.
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------
#pragma once

#include "advanced_base.hpp"

namespace xpcc {

namespace stm32 {

/**
 * @brief		Advanced Control Timer x
 * 
 * {% if id == 1 %}
 * Interrupt handler (for LD, MD, HD and CL):
 * - TIM1_BRK_IRQHandler
 * - TIM1_UP_IRQHandler
 * - TIM1_TRG_COM_IRQHandler
 * - TIM1_CC_IRQHandler
 * 
 * Interrupt handler for XL Density:
 * - TIM1_BRK_TIM9_IRQHandler
 * - TIM1_UP_TIM10_IRQn
 * - TIM1_TRG_COM_TIM11_IRQn
 * - TIM1_CC_IRQn
 * {% elif id == 8 %}
 * Interrupt handler for High Density:
 * - TIM8_BRK_IRQHandler
 * - TIM8_UP_IRQHandler
 * - TIM8_TRG_COM_IRQHandler
 * - TIM8_CC_IRQHandler
 * 
 * Interrupt handler for XL Density:
 * - TIM8_BRK_TIM12_IRQn
 * - TIM8_UP_TIM13_IRQn
 * - TIM8_TRG_COM_TIM14_IRQn
 * - TIM8_CC_IRQn
 * {% endif %}
 * 
 * Example:
 * \code
 * extern "C" void
 * TIMx_UP_IRQHandler(void)
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
 * @author		Kevin Laeufer
 * @ingroup		stm32
 */

TIM_TypeDef* const adv_timers[] = { TIM1, 0, 0, 0, 0, 0, 0, TIM8, 0, 0, 0, 0, 0,
		0 };


#define TIMx (adv_timers[timid-1])

template<int timid>
class AdvTimer: public AdvancedControlTimer {
public:

public:
	static void
	enable() {
		if(timid == 1) {
			// enable clock
			RCC->APB2ENR  |=  RCC_APB2ENR_TIM1EN;

			// reset timer
			RCC->APB2RSTR |=  RCC_APB2RSTR_TIM1RST;
			RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM1RST;
		} else
		if(timid == 8) {
			// enable clock
			RCC->APB2ENR  |=  RCC_APB2ENR_TIM8EN;

			// reset timer
			RCC->APB2RSTR |=  RCC_APB2RSTR_TIM8RST;
			RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM8RST;
		}
	}

	static void
	disable() {
		if(timid == 1) {
			// disable clock
			RCC->APB2ENR &= ~RCC_APB2ENR_TIM1EN;
		} else
		if(timid == 8) {
			// enable clock
			RCC->APB2ENR  &= ~RCC_APB2ENR_TIM8EN;
		}

		TIMx->CR1 = 0;
		TIMx->DIER = 0;
		TIMx->CCER = 0;
	}


	static inline void pause() {
		TIMx->CR1 &= ~TIM_CR1_CEN;
	}

	static inline void start() {
		TIMx->CR1 |= TIM_CR1_CEN;
	}

	static void setMode(Mode mode, SlaveMode slaveMode = SlaveMode::Disabled,
			SlaveModeTrigger slaveModeTrigger = SlaveModeTrigger::Internal0,
			MasterMode masterMode = MasterMode::Reset
#if defined(STM32F3XX)
			, MasterMode2 masterMode2 = MasterMode2::Reset
#endif
			) {
		// disable timer
		TIMx->CR1 = 0;
		TIMx->CR2 = 0;

		if (slaveMode == SlaveMode::Encoder1 || slaveMode == SlaveMode::Encoder2
				|| slaveMode == SlaveMode::Encoder3) {
			setPrescaler(1);
		}

		// ARR Register is buffered, only Under/Overflow generates update interrupt
		TIMx->CR1 = TIM_CR1_ARPE | TIM_CR1_URS | static_cast<uint32_t>(mode);
#if defined(STM32F3XX)
		TIMx->CR2 = static_cast<uint32_t>(masterMode) |
		static_cast<uint32_t>(masterMode2);
#else
		TIMx->CR2 = static_cast<uint32_t>(masterMode);
#endif
		TIMx->SMCR = static_cast<uint32_t>(slaveMode)
				| static_cast<uint32_t>(slaveModeTrigger);
	}

	static inline void setCaptureCompareControlUpdate(CaptureCompareControlUpdate update) {
		TIMx->CR2 = (TIMx->CR2 & ~TIM_CR2_CCUS) | static_cast<uint32_t>(update);
	}

	/*
	 * Enables Capture/Compare preloaded control
	 *
	 * If enabled CCxE, CCxNE and OCxM bits are preloaded and only
	 * updated when the COMG bit is set or on a rising edge on TRGI
	 * This is determined by the CaptureCompareControlUpdate setting.
	 */
	static inline void enableCaptureComparePreloadedControl(
			CaptureCompareControlUpdate update =
					CaptureCompareControlUpdate::SetComg) {
		TIMx->CR2 = (TIMx->CR2 & ~TIM_CR2_CCUS) | static_cast<uint32_t>(update)
				| TIM_CR2_CCPC;
	}

	static inline void disableCaptureComparePreloadedControl() {
		TIMx->CR2 &= ~TIM_CR2_CCPC;
	}

	static inline void setPrescaler(uint16_t prescaler) {
		// Because a prescaler of zero is not possible the actual
		// prescaler value is \p prescaler - 1 (see Datasheet)
		TIMx->PSC = prescaler - 1;
	}

	static inline void setOverflow(uint16_t overflow) {
		TIMx->ARR = overflow;
	}

	static uint16_t setPeriod(uint32_t microseconds, bool autoApply = true) {
		// This will be inaccurate for non-smooth frequencies (last six digits
		// unequal to zero)
		uint32_t cycles = microseconds * (Clocks::getPCLK2Frequency()*2 / 1000000UL);
		uint16_t prescaler = (cycles + 65535) / 65536;	// always round up
		uint16_t overflow = cycles / prescaler;

		overflow = overflow - 1;	// e.g. 36000 cycles are from 0 to 35999

		setPrescaler(prescaler);
		setOverflow(overflow);

		if (autoApply) {
			// Generate Update Event to apply the new settings for ARR
			TIMx->EGR |= TIM_EGR_UG;
		}

		return overflow;
	}

	static inline void applyAndReset() {
		// Generate Update Event to apply the new settings for ARR
		generateEvent(Event::Update);
	}

	static inline void generateEvent(Event ev) {
		TIMx->EGR |= static_cast<uint32_t>(ev);
	}

	static inline uint16_t getValue() {
		return TIMx->CNT;
	}

	static inline void setValue(uint16_t value) {
		TIMx->CNT = value;
	}

	static inline void enableOutput() {
		TIMx->BDTR |= TIM_BDTR_MOE;
	}

	/*
	 * Enable/Disable automatic set of MOE bit at the next update event
	 */
	static inline void setAutomaticUpdate(bool enable) {
		if (enable)
			TIMx->BDTR |= TIM_BDTR_AOE;
		else
			TIMx->BDTR &= ~TIM_BDTR_AOE;
	}

	static inline void setOffState(OffStateForRunMode runMode,
			OffStateForIdleMode idleMode) {
		uint32_t flags = TIMx->BDTR;
		flags &= ~(TIM_BDTR_OSSR | TIM_BDTR_OSSI);
		flags |= static_cast<uint32_t>(runMode);
		flags |= static_cast<uint32_t>(idleMode);
		TIMx->BDTR = flags;
	}

	static inline void setOutputIdleState(uint32_t channel,
			OutputIdleState idle, OutputIdleState idle_n =
					OutputIdleState::Reset) {
		uint32_t flags = TIMx->CR2;
		channel -= 1;
		flags &= (static_cast<uint32_t>(OutputIdleState::Set) << (channel * 2))
				| (static_cast<uint32_t>(OutputIdleState::Set)
						<< (channel * 2 + 1));
		flags |= (static_cast<uint32_t>(idle) << (channel * 2));
		flags |= (static_cast<uint32_t>(idle_n) << (channel * 2 + 1));
		TIMx->CR2 = flags;
	}

	/*
	 * Set Dead Time Value
	 *
	 * Different Resolution Depending on DeadTime[7:5]:
	 *     0xx =>  DeadTime[6:0]            * T(DTS)
	 *     10x => (DeadTime[5:0] + 32) *  2 * T(DTS)
	 *     110 => (DeadTime[4:0] + 4)  *  8 * T(DTS)
	 *     111 => (DeadTime[4:0] + 2)  * 16 * T(DTS)
	 */
	static inline void setDeadTime(uint8_t deadTime) {
		uint32_t flags = TIMx->BDTR;
		flags &= TIM_BDTR_DTG;
		flags |= deadTime;
		TIMx->BDTR = flags;
	}

	/*
	 * Set Dead Time Value
	 *
	 * Different Resolution Depending on DeadTime[7:5]:
	 *     0xx =>  DeadTime[6:0]            * T(DTS)
	 *     10x => (DeadTime[5:0] + 32) *  2 * T(DTS)
	 *     110 => (DeadTime[4:0] + 4)  *  8 * T(DTS)
	 *     111 => (DeadTime[4:0] + 2)  * 16 * T(DTS)
	 */
	static inline void setDeadTime(DeadTimeResolution resolution,
			uint8_t deadTime) {
		uint8_t bitmask;
		switch (resolution) {
		case DeadTimeResolution::From0With125nsStep:
			bitmask = 0b01111111;
			break;
		case DeadTimeResolution::From16usWith250nsStep:
			bitmask = 0b00111111;
			break;
		case DeadTimeResolution::From32usWith1usStep:
		case DeadTimeResolution::From64usWith2usStep:
			bitmask = 0b00011111;
			break;
		default:
			bitmask = 0x00;
			break;
		}
		uint32_t flags = TIMx->BDTR;
		flags &= TIM_BDTR_DTG;
		flags |= (deadTime & bitmask) | static_cast<uint32_t>(resolution);
		TIMx->BDTR = flags;
	}

public:
	static void configureInputChannel(uint32_t channel,
			InputCaptureMapping input, InputCapturePrescaler prescaler,
			InputCapturePolarity polarity, uint8_t filter, bool xor_ch1_3 =
					false) {
		channel -= 1;	// 1..4 -> 0..3

		// disable channel
		TIMx->CCER &= ~((TIM_CCER_CC1NP | TIM_CCER_CC1P | TIM_CCER_CC1E)
				<< (channel * 4));

		uint32_t flags = static_cast<uint32_t>(input);
		flags |= static_cast<uint32_t>(prescaler) << 2;
		flags |= static_cast<uint32_t>(filter & 0xf) << 4;

		if (channel <= 1) {
			uint32_t offset = 8 * channel;

			flags <<= offset;
			flags |= TIMx->CCMR1 & ~(0xff << offset);

			TIMx->CCMR1 = flags;

			if (channel == 0) {
				if (xor_ch1_3)
					TIMx->CR2 |= TIM_CR2_TI1S;
				else
					TIMx->CR2 &= ~TIM_CR2_TI1S;
			}
		} else {
			uint32_t offset = 8 * (channel - 2);

			flags <<= offset;
			flags |= TIMx->CCMR2 & ~(0xff << offset);

			TIMx->CCMR2 = flags;
		}

		TIMx->CCER |= (TIM_CCER_CC1E | static_cast<uint32_t>(polarity))
				<< (channel * 4);
	}

	static void configureOutputChannel(uint32_t channel, OutputCompareMode mode,
			uint16_t compareValue) {
		channel -= 1;	// 1..4 -> 0..3

		// disable output
		TIMx->CCER &= ~(0xf << (channel * 4));

		setCompareValue(channel + 1, compareValue);

		// enable preload (the compare value is loaded at each update event)
		uint32_t flags = static_cast<uint32_t>(mode) | TIM_CCMR1_OC1PE;

		if (channel <= 1) {
			uint32_t offset = 8 * channel;

			flags <<= offset;
			flags |= TIMx->CCMR1 & ~(0xff << offset);

			TIMx->CCMR1 = flags;
		} else {
			uint32_t offset = 8 * (channel - 2);

			flags <<= offset;
			flags |= TIMx->CCMR2 & ~(0xff << offset);

			TIMx->CCMR2 = flags;
		}

		// Disable Repetition Counter (FIXME has to be done here for some unknown reason)
		TIMx->RCR = 0;

		if (mode != OutputCompareMode::Inactive) {
			TIMx->CCER |= (TIM_CCER_CC1E) << (channel * 4);
		}
	}

	// TODO: Maybe add some functionality from the configureOutput
	//       function below...

	/*
	 * Configure Output Channel without changing the Compare Value
	 *
	 * Normally used to REconfigure the Output channel without touching
	 * the compare value. This can e.g. be useful for commutation of a
	 * bldc motor.
	 *
	 * This function probably won't be used for a one time setup but
	 * rather for adjusting the output setting periodically.
	 * Therefore it aims aims to provide the best performance possible
	 * without sacrificing code readability.
	 */
	static void configureOutputChannel(uint32_t channel, OutputCompareMode mode,
			PinState out, OutputComparePolarity polarity, PinState out_n,
			OutputComparePolarity polarity_n = OutputComparePolarity::ActiveHigh,
			OutputComparePreload preload = OutputComparePreload::Disable) {
		channel -= 1;	// 1..4 -> 0..3

		// disable output
		TIMx->CCER &= ~(0xf << (channel * 4));

		uint32_t flags = static_cast<uint32_t>(mode)
				| static_cast<uint32_t>(preload);

		if (channel <= 1) {
			uint32_t offset = 8 * channel;

			flags <<= offset;
			flags |= TIMx->CCMR1 & ~(0xff << offset);

			TIMx->CCMR1 = flags;
		} else {
			uint32_t offset = 8 * (channel - 2);

			flags <<= offset;
			flags |= TIMx->CCMR2 & ~(0xff << offset);

			TIMx->CCMR2 = flags;
		}

		// Disable Repetition Counter (FIXME has to be done here for some unknown reason)
		TIMx->RCR = 0;

		// CCER Flags (Enable/Polarity)
		flags = (static_cast<uint32_t>(polarity_n) << 2)
				| (static_cast<uint32_t>(out_n) << 2)
				| static_cast<uint32_t>(polarity) | static_cast<uint32_t>(out);

		TIMx->CCER |= flags << (channel * 4);
	}

	/*
	 * Configure Output Channel width Mode/OutputPort uint
	 *
	 * This is the least typesafe way of doing this and should only
	 * be done if it provides a necessary performance
	 * (or more or less) laziness benefit.
	 * i.e. if you have specific mode/output uints precalculated and
	 * just want to load them as fast as possible.
	 *
	 * The "mode/output" uint contains four bits
	 * that describe the intended output setting:
	 * Bit0: Output Enabled/Disabled
	 * Bit1: Output Polarity
	 * Bit2: Complementary Output Enable/Disable
	 * Bit3: Complementary Output Polarity
	 *
	 * As well as Mode Information (Bit6-Bit4)
	 * which is just an OutputCompareMode constant ored with the
	 * port output quadruple specified above.
	 */
	static void configureOutputChannel(uint32_t channel,
			uint32_t modeOutputPorts) {
		channel -= 1;	// 1..4 -> 0..3

		// disable output
		TIMx->CCER &= ~(0xf << (channel * 4));

		uint32_t flags = modeOutputPorts & (0x70);

		if (channel <= 1) {
			uint32_t offset = 8 * channel;

			flags <<= offset;
			flags |= TIMx->CCMR1 & ~(TIM_CCMR1_OC1M << offset);
			TIMx->CCMR1 = flags;
		} else {
			uint32_t offset = 8 * (channel - 2);

			flags <<= offset;
			flags |= TIMx->CCMR2 & ~(TIM_CCMR1_OC1M << offset);

			TIMx->CCMR2 = flags;
		}

		// Disable Repetition Counter (FIXME has to be done here for some unknown reason)
		TIMx->RCR = 0;

		TIMx->CCER |= (modeOutputPorts & (0xf)) << (channel * 4);
	}

	static inline void setCompareValue(uint32_t channel, uint16_t value) {
#if defined(STM32F2XX) || defined(STM32F3XX) || defined(STM32F4XX)
		*(&TIMx->CCR1 + (channel - 1)) = value;
#else
		*(&TIMx->CCR1 + ((channel - 1) * 2)) = value;
#endif
	}

	static inline uint16_t getCompareValue(uint32_t channel) {
#if defined(STM32F2XX) || defined(STM32F3XX) || defined(STM32F4XX)
		return *(&TIMx->CCR1 + (channel - 1));
#else
		return *(&TIMx->CCR1 + ((channel - 1) * 2));
#endif
	}

public:
	/**
	 * Enables or disables Interrupt Vectors.
	 * 
	 * You must pass an or-conjuncted list of entries from the
	 * Interrupt enum. Interrupt vectors which fit to 
	 * will be enabled (or disabled if \p enable = false).
	 * 
	 * The Adanvced timers have four interrupt vectors:
	 * - UP (used by INTERRUPT_UPDATE)
	 * - BRK (used by INTERRUPT_BREAK)
	 * - TRG_COM (used by INTERRUPT_TRIGGER and INTERRUPT_COM)
	 * - CC (used by INTERRUPT_CAPTURE_COMPARE_1..3)
	 */
	static void
	enableInterruptVector(Interrupt interrupt, bool enable, uint32_t priority) {
		uint32_t TIMx_UP_IRQn;
		uint32_t TIMx_BRK_IRQn;
		uint32_t TIMx_TRG_COM_IRQn;
		uint32_t TIMx_CC_IRQn;
		if (timid == 1) {
			TIMx_BRK_IRQn = TIM1_BRK_TIM9_IRQn;
			TIMx_UP_IRQn = TIM1_UP_TIM10_IRQn;
			TIMx_TRG_COM_IRQn = TIM1_TRG_COM_TIM11_IRQn;
		} else if (timid == 8) {
			TIMx_BRK_IRQn = TIM8_BRK_TIM12_IRQn;
			TIMx_UP_IRQn = TIM8_UP_TIM13_IRQn;
			TIMx_TRG_COM_IRQn = TIM8_TRG_COM_TIM14_IRQn;
		}

		if (enable)
		{
			if (interrupt & Interrupt::Update) {
				NVIC_SetPriority(TIMx_UP_IRQn, priority);
				NVIC_EnableIRQ(TIMx_UP_IRQn);
			}

			if (interrupt & Interrupt::Break) {
				NVIC_SetPriority(TIMx_BRK_IRQn, priority);
				NVIC_EnableIRQ(TIMx_BRK_IRQn);
			}

			if (interrupt & (Interrupt::COM | Interrupt::Trigger)) {
				NVIC_SetPriority(TIMx_TRG_COM_IRQn, priority);
				NVIC_EnableIRQ(TIMx_TRG_COM_IRQn);
			}

			if (interrupt &
					(Interrupt::CaptureCompare1 | Interrupt::CaptureCompare2 |
					 Interrupt::CaptureCompare3 | Interrupt::CaptureCompare4)) {
				NVIC_SetPriority(TIMx_CC_IRQn, priority);
				NVIC_EnableIRQ(TIMx_CC_IRQn);
			}
		}
		else
		{
			if (interrupt & Interrupt::Update) {
				NVIC_DisableIRQ(TIMx_UP_IRQn);
			}

			if (interrupt & Interrupt::Break) {
				NVIC_DisableIRQ(TIMx_BRK_IRQn);
			}

			if (interrupt & (Interrupt::COM | Interrupt::Trigger)) {
				NVIC_DisableIRQ(TIMx_TRG_COM_IRQn);
			}

			if (interrupt &
					(Interrupt::CaptureCompare1 | Interrupt::CaptureCompare2 |
					 Interrupt::CaptureCompare3 | Interrupt::CaptureCompare4)) {
				NVIC_DisableIRQ(TIMx_CC_IRQn);
			}
		}
	}

	static inline void enableInterrupt(Interrupt interrupt) {
		TIMx->DIER |= static_cast<uint32_t>(interrupt);
	}

	static inline void disableInterrupt(Interrupt interrupt) {
		TIMx->DIER &= ~static_cast<uint32_t>(interrupt);
	}

	static inline void enableDmaRequest(DmaRequestEnable dmaRequests) {
		TIMx->DIER |= static_cast<uint32_t>(dmaRequests);
	}

	static inline void disableDmaRequest(DmaRequestEnable dmaRequests) {
		TIMx->DIER &= ~static_cast<uint32_t>(dmaRequests);
	}

	static inline InterruptFlag getInterruptFlags() {
		return static_cast<InterruptFlag>(TIMx->SR);
	}

	static inline void acknowledgeInterruptFlags(InterruptFlag flags) {
		// Flags are cleared by writing a zero to the flag position.
		// Writing a one is ignored.
		TIMx->SR = ~static_cast<uint32_t>(flags);
	}
};

typedef AdvTimer<1> AdvTimer1;
typedef AdvTimer<8> AdvTimer8;

}
}

#undef TIMx
