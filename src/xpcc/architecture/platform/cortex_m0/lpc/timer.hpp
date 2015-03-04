/*
 * timer32.hpp
 *
 *  Created on: Apr 3, 2013
 *      Author: walmis
 */

#ifndef TIMER32_HPP_
#define TIMER32_HPP_

//#include <xpcc/architecture.hpp>
#ifdef __ARM_LPC11UXX__
#include <lpc11xx/cmsis/LPC11Uxx.h>
#define TIMER reinterpret_cast<LPC_CTxxBx_Type*>(timerptr)
#define LPC_TMR16B0 LPC_CT16B0
#define LPC_TMR16B1 LPC_CT16B1
#define LPC_TMR32B0 LPC_CT32B0
#define LPC_TMR32B1 LPC_CT32B1
#else
#include <lpc11xx/cmsis/LPC11xx.h>
#define TIMER reinterpret_cast<LPC_TMR_TypeDef*>(timerptr)
#endif


#include "timer_defs.hpp"
#include "iocon.hpp"




namespace xpcc {
namespace lpc11 {
/** @brief Timer/counter operating mode */
enum class TimerMode
{
	TIMER_MODE = 0,				/*!< Timer mode */
	COUNTER_RISING_MODE,		/*!< Counter rising mode */
	COUNTER_FALLING_MODE,		/*!< Counter falling mode */
	COUNTER_ANY_MODE			/*!< Counter on both edges */
};

enum class PrescaleMode
{
	PRESCALE_TICKVAL = 0,		/*!< Prescale in absolute value */
	PRESCALE_USVAL				/*!< Prescale in microsecond value */
};

enum class CapInput {
	COUNTER_INCAP0 = 0,			/*!< CAPn.0 input pin for TIMERn */
	COUNTER_INCAP1,				/*!< CAPn.1 input pin for TIMERn */
};

enum CaptureFlags : uint8_t {
	DISABLED = 0,
	//capture on rising edge
	RISING_EDGE = 1<<0,
	//capture on falling edge
	FALLING_EDGE = 1<<1,
	//interrupt on capture
	INT_ON_CAPTURE = 1<<2
};

enum class MatchPins {
	PINS_MAT0 = 0,	/*!< External Match Output 0 */
	PINS_MAT1,      /*!< External Match Output 1 */
	PINS_MAT2,      /*!< External Match Output 2 */
	PINS_MAT3,      /*!< External Match Output 3 */
};

enum class ExtMatchOpt
{
	EXTMATCH_NOTHING = 0,		/*!< Do nothing for external output pin if match */
	EXTMATCH_LOW,				/*!< Force external output pin to low if match */
	EXTMATCH_HIGH,				/*!< Force external output pin to high if match */
	EXTMATCH_TOGGLE				/*!< Toggle external output pin if match */
};

enum class TmrIntType
{
	MR0_INT =0, /*!< interrupt for Match channel 0*/
	MR1_INT =1, /*!< interrupt for Match channel 1*/
	MR2_INT =2, /*!< interrupt for Match channel 2*/
	MR3_INT =3, /*!< interrupt for Match channel 3*/
	CR0_INT =4, /*!< interrupt for Capture channel 0*/
};


template <int timerptr>
class Timer {

public:
		/*********************************************************************//**
		 * @brief 		Assign External Match Output Pins
		 * @param[in]	timer Pointer to timer/counter device, can be LPC_TMR16B0,
		 *                   LPC_TMR16B1, LPC_TMR32B0, LPC_TMR32B1, pins assgign:
		 *                  16-bit Counter/Timer 0
		 *                      - MAT0 : PIO0_8
		 *                      - MAT1 : PIO0_9
		 *                      - MAT2 : PIO0_10
		 *                      - MAT3 : None pin out
		 *                  16-bit Counter/Timer 1
		 *                      - MAT0 : PIO1_9
		 *                      - MAT1 : PIO1_10
		 *                      - MAT2 : None pin out
		 *                      - MAT3 : None pin out
		 *                  32-bit Counter/Timer 0
		 *                      - MAT0 : PIO1_6
		 *                      - MAT1 : PIO1_7
		 *                      - MAT2 : PIO0_1
		 *                      - MAT3 : PIO0_11
		 *                  32-bit Counter/Timer 1
		 *                      - MAT0 : PIO1_1
		 *                      - MAT1 : PIO1_2
		 *                      - MAT2 : PIO1_3
		 *                      - MAT3 : PIO1_4
		 * @param[in]	matFlag, external Match Output, can be:
		 *                   - TIM_PINS_MAT0 : External Match Output 0
		 *                   - TIM_PINS_MAT1 : External Match Output 1
		 *                   - TIM_PINS_MAT2 : External Match Output 2
		 *                   - TIM_PINS_MAT3 : External Match Output 3
		 * @return 		None
		 **********************************************************************/

		static void
		ALWAYS_INLINE
		assignMatchPins(MatchPins matchPins);


		static void
		ALWAYS_INLINE
		initPWM(uint32_t match_value, uint8_t main_channel = 3) {
			//set pwm match channel
			configureMatch(main_channel, match_value);

		}

		static void
		ALWAYS_INLINE
		initPWMChannel(uint8_t channel) {
			//enable pwm channel
			TIMER->PWMC |= 1<<channel;

			//enable match outputs
			assignMatchPins((MatchPins)channel);

		}

		static void
		ALWAYS_INLINE
		setPWM(uint8_t channel, uint32_t value) {
			switch(channel)
			{
			case 0:
				TIMER->MR0 = value;
				break;
			case 1:
				TIMER->MR1 = value;
				break;
		    case 2:
				TIMER->MR2 = value;
				break;
		    case 3:
				TIMER->MR3 = value;
				break;
			}
		}

		/*********************************************************************//**
		 * @brief 		Get Interrupt Status
		 * @param[in]	timer Timer selection, should be LPC_TMR16B0,
		 *                   LPC_TMR16B1, LPC_TMR32B0, LPC_TMR32B1
		 * @param[in]	IntFlag
		 * @return 		FlagStatus
		 * 				- SET : interrupt
		 * 				- RESET : no interrupt
		 **********************************************************************/
		static bool
		ALWAYS_INLINE
		getIntStatus(TmrIntType IntFlag)
		{

			if ((TIMER->IR)& TIM_IR_CLR((int)IntFlag)) {
		        return true;
		    }

			return false;

		}

		/*********************************************************************//**
		 * @brief 		Configuration for Match register
		 * @param[in]	timer Pointer to timer device
		 * @param[in]   TIM_MatchConfigStruct Pointer to TIM_MATCHCFG_Type
		 * 					- MatchChannel : choose channel 0 or 1
		 * 					- IntOnMatch	 : if SET, interrupt will be generated when MRxx match
		 * 									the value in TC
		 * 					- StopOnMatch	 : if SET, TC and PC will be stopped whenM Rxx match
		 * 									the value in TC
		 * 					- ResetOnMatch : if SET, Reset on MR0 when MRxx match
		 * 									the value in TC
		 * 					-ExtMatchOutputType: Select output for external match
		 * 						 +	 0:	Do nothing for external output pin if match
		 *						 +   1:	Force external output pin to low if match
		 *						 + 	 2: Force external output pin to high if match
		 *						 + 	 3: Toggle external output pin if match
		 *					MatchValue: Set the value to be compared with TC value
		 * @return 		None
		 **********************************************************************/
		static void
		ALWAYS_INLINE
		configureMatch(uint32_t matchChannel,
								uint32_t matchValue,
								ExtMatchOpt extMatch = ExtMatchOpt::EXTMATCH_NOTHING,
								bool resetOnMatch = true,
								bool stopOnMatch = false,
								bool intOnMatch = false)
		{

			switch(matchChannel)
			{
			case 0:
				TIMER->MR0 = matchValue;
				break;
			case 1:
				TIMER->MR1 = matchValue;
				break;
		    case 2:
				TIMER->MR2 = matchValue;
				break;
		    case 3:
				TIMER->MR3 = matchValue;
				break;
			}

			TIMER->MCR &=~TIM_MCR_CHANNEL_MASKBIT(matchChannel);

			if (intOnMatch)
				TIMER->MCR |= TIM_INT_ON_MATCH(matchChannel);

			//reset on MRn
			if (resetOnMatch)
				TIMER->MCR |= TIM_RESET_ON_MATCH(matchChannel);

			//stop on MRn
			if (stopOnMatch)
				TIMER->MCR |= TIM_STOP_ON_MATCH(matchChannel);

			// match output type

			TIMER->EMR &= ~TIM_EM_MASK(matchChannel);
			TIMER->EMR |= TIM_EM_SET(matchChannel, (int)extMatch);
		}

		static void
		ALWAYS_INLINE
		updateMatchValue(uint32_t matchChannel,	uint32_t matchValue)
		{
			switch(matchChannel)
			{
			case 0:
				TIMER->MR0 = matchValue;
				break;
			case 1:
				TIMER->MR1 = matchValue;
				break;
		    case 2:
				TIMER->MR2 = matchValue;
				break;
		    case 3:
				TIMER->MR3 = matchValue;
				break;
			}
		}

		/*********************************************************************//**
		 * @brief 		Clear Interrupt pending
		 * @param[in]	timer Timer selection, should be LPC_TMR16B0,
		 *                   LPC_TMR16B1, LPC_TMR32B0, LPC_TMR32B1
		 * @param[in]	IntFlag should be in TIM_INT_TYPE enum
		 * @return 		None
		 **********************************************************************/
		static void
		ALWAYS_INLINE
		clearIntPending(TmrIntType IntFlag)
		{

			TIMER->IR = TIM_IR_CLR((int)IntFlag);
		}

		/*********************************************************************//**
		 * @brief	 	Start/Stop Timer/Counter device
		 * @param[in]	timer Pointer to timer device, should be LPC_TMR16B0,
		 *                    LPC_TMR16B1, LPC_TMR32B0, LPC_TMR32B1
		 * @param[in]	NewState
		 * 				-	ENABLE  : set timer enable
		 * 				-	DISABLE : disable timer
		 * @return 		None
		 **********************************************************************/
		static void
		ALWAYS_INLINE
		activate(bool enable = true)
		{
			if (enable) {
				TIMER->TCR	|=  TIM_ENABLE;
			} else {
				TIMER->TCR &= ~TIM_ENABLE;
			}
		}

		static bool
		ALWAYS_INLINE
		isActive()
		{
			return TIMER->TCR & TIM_ENABLE;
		}

		/*********************************************************************//**
		 * @brief 		Reset Timer/Counter device,
		 * 					Make TC and PC are synchronously reset on the next
		 * 					positive edge of PCLK
		 * @param[in]	timer Pointer to timer device, should be LPC_TMR16B0,
		 *                    LPC_TMR16B1, LPC_TMR32B0, LPC_TMR32B1
		 * @return 		None
		 **********************************************************************/
		static void
		ALWAYS_INLINE
		resetCounter()
		{
			TIMER->TCR |= TIM_RESET;
			TIMER->TCR &= ~TIM_RESET;
		}

		/*********************************************************************//**
		 * @brief 		Close Timer/Counter device
		 * @param[in]	timer  Pointer to timer device, should be LPC_TMR16B0,
		 *                    LPC_TMR16B1, LPC_TMR32B0, LPC_TMR32B1
		 * @return 		None
		 **********************************************************************/
		static void
		ALWAYS_INLINE
		deinit ()
		{
			// Disable timer/counter
			TIMER->TCR = 0x00;
			// Disable power
		    if (TIMER == LPC_TMR16B0) {    //  16-bit counter/TIMER 0
		    	LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<7);
		    } else if(TIMER == LPC_TMR16B1) {    //  16-bit counter/timer 1
		    	LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<8);
		    } else if(TIMER == LPC_TMR32B0) {    //  32-bit counter/timer 0
		    	LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<9);
		    } else if(TIMER == LPC_TMR32B1) {    //  32-bit counter/timer 1
		    	LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<10);
		    }
		}

		static uint32_t
		ALWAYS_INLINE
		getCounterValue() {
			return TIMER->TC;
		}



		/*********************************************************************//**
		 * @brief 		Initial Timer/Counter device
		 * 				 	Set Clock frequency for ADC
		 * 					Set initial configuration for ADC
		 * @param[in]	timer  Timer selection, should be LPC_TMR16B0, LPC_TMR16B1
		 *                    LPC_TMR32B0, LPC_TMR32B1
		 * @param[in]	TimerCounterMode TIM_MODE_OPT
		 * @param[in]	TIM_ConfigStruct pointer to TIM_TIMERCFG_Type
		 * 				that contains the configuration information for the
		 *                    specified Timer peripheral.
		 * @return 		None
		 **********************************************************************/
		static void
		ALWAYS_INLINE
		init(TimerMode TimerCounterMode, uint32_t prescale_value,
				PrescaleMode prescMode = PrescaleMode::PRESCALE_TICKVAL)
		{
		    uint32_t val;


			// Set power
		    if(TIMER == LPC_TMR16B0) {
		        LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);

		    } else if(TIMER == LPC_TMR16B1) {
		    	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<8);

		    } else if(TIMER == LPC_TMR32B0) {
		    	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<9);

		    } else if(TIMER == LPC_TMR32B1) {
		    	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<10);
		    }

			TIMER->CTCR &= ~TIM_CTCR_MODE_MASK;
			TIMER->CTCR |= (int)TimerCounterMode;


			TIMER->TC =0;
			TIMER->PC =0;
			TIMER->PR =0;

			if (prescMode  == PrescaleMode::PRESCALE_TICKVAL ||
					TimerCounterMode != TimerMode::TIMER_MODE) {
				val = prescale_value -1  ;
			} else {
				val = TIM_ConverUSecToVal (prescale_value)-1;
			}

			if ((TIMER == LPC_TMR16B0) || (TIMER == LPC_TMR16B1)) {
				val &= 0xFFFF;
			}
			TIMER->PR = val;

			// Clear interrupt pending
			TIMER->IR = 0x3F;
		}

		static void
		ALWAYS_INLINE
		configCapture(CaptureFlags flags)
		{

			TIMER->CCR &= ~TIM_CCR_CHANNEL_MASKBIT(0);

			if (flags & (CaptureFlags::RISING_EDGE)) {
		        TIMER->CCR |= TIM_CAP_RISING(0);
		    }

			if (flags & (CaptureFlags::FALLING_EDGE)) {
		        TIMER->CCR |= TIM_CAP_FALLING(0);
		    }

			if (flags & (CaptureFlags::INT_ON_CAPTURE)) {
		        TIMER->CCR |= TIM_INT_ON_CAP(0);
		    }
		}

		/*********************************************************************//**
		 * @brief 		Read value of capture register in timer/counter device
		 * @param[in]	timer Pointer to timer/counter device, can be LPC_TMR16B0,
		 *                   LPC_TMR16B1, LPC_TMR32B0, LPC_TMR32B1
		 * @return 		Value of capture register
		 **********************************************************************/
		static uint32_t
		ALWAYS_INLINE
		getCaptureValue()
		{
		    uint32_t val;

		    val = TIMER->CR0;
			if((TIMER == LPC_TMR16B0) || (TIMER == LPC_TMR16B1)) {
		        val &= 0xFFFF;
		    }

		    return val;
		}

		/*********************************************************************//**
		 * @brief 		Assign Capture Signals
		 *                - CT16B0_CAP0 : PIO0_2
		 *                - CT16B1_CAP0 : PIO1_8
		 *                - CT32B0_CAP0 : PIO1_5
		 *                - CT32B1_CAP0 : PIO1_0
		 * @param[in]	timer Pointer to timer/counter device, can be LPC_TMR16B0,
		 *                   LPC_TMR16B1, LPC_TMR32B0, LPC_TMR32B1
		 * @return 		None
		 **********************************************************************/
		static void
		ALWAYS_INLINE
		enableCapturePins();

		static void
		ALWAYS_INLINE
		enableIRQ() {
		    if (TIMER == LPC_TMR16B0) {    //  16-bit counter/TIMER 0
		    	NVIC_EnableIRQ(TIMER_16_0_IRQn);
		    } else if(TIMER == LPC_TMR16B1) {    //  16-bit counter/timer 1
		    	NVIC_EnableIRQ(TIMER_16_1_IRQn);
		    } else if(TIMER == LPC_TMR32B0) {    //  32-bit counter/timer 0
		    	NVIC_EnableIRQ(TIMER_32_0_IRQn);
		    } else if(TIMER == LPC_TMR32B1) {    //  32-bit counter/timer 1
		    	NVIC_EnableIRQ(TIMER_32_1_IRQn);
		    }
		}

		static void
		ALWAYS_INLINE
		disableIRQ() {
		    if (TIMER == LPC_TMR16B0) {    //  16-bit counter/TIMER 0
		    	NVIC_DisableIRQ(TIMER_16_0_IRQn);
		    } else if(TIMER == LPC_TMR16B1) {    //  16-bit counter/timer 1
		    	NVIC_DisableIRQ(TIMER_16_1_IRQn);
		    } else if(TIMER == LPC_TMR32B0) {    //  32-bit counter/timer 0
		    	NVIC_DisableIRQ(TIMER_32_0_IRQn);
		    } else if(TIMER == LPC_TMR32B1) {    //  32-bit counter/timer 1
		    	NVIC_DisableIRQ(TIMER_32_1_IRQn);
		    }
		}


private:
		static uint32_t TIM_ConverUSecToVal (uint32_t usec)
		{
			uint64_t clkdlycnt;

			// Get Pclock of timer
			clkdlycnt = SystemCoreClock / LPC_SYSCON->SYSAHBCLKDIV;

			clkdlycnt = (clkdlycnt * usec) / 1000000;
			return (uint32_t) clkdlycnt;
		}


	};


typedef Timer<(int)LPC_TMR16B0> Timer16B0;
typedef Timer<(int)LPC_TMR16B1> Timer16B1;
typedef Timer<(int)LPC_TMR32B0> Timer32B0;
typedef Timer<(int)LPC_TMR32B1> Timer32B1;


}
}



#endif /* TIMER32_HPP_ */
