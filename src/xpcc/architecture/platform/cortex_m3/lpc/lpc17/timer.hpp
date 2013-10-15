/*
 * timer.hpp
 *
 *  Created on: Oct 15, 2013
 *      Author: walmis
 */

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <xpcc/architecture.hpp>

namespace xpcc {

namespace lpc17 {

#define TIMx (reinterpret_cast<LPC_TIM_TypeDef*>(timerptr))

#define _BIT(n)	(1<<n)
#define _SBF(f,v) (v<<f)

/* --------------------- BIT DEFINITIONS -------------------------------------- */
/**********************************************************************
 ** Interrupt information
 **********************************************************************/
/** Macro to clean interrupt pending */
#define TIM_IR_CLR(n) _BIT(n)

/**********************************************************************
 ** Timer interrupt register definitions
 **********************************************************************/
/** Macro for getting a timer match interrupt bit */
#define TIM_MATCH_INT(n)		(_BIT(n & 0x0F))
/** Macro for getting a capture event interrupt bit */
#define TIM_CAP_INT(n)     (_BIT(((n & 0x0F) + 4)))

/**********************************************************************
 * Timer control register definitions
 **********************************************************************/
/** Timer/counter enable bit */
#define TIM_ENABLE			((uint32_t)(1<<0))
/** Timer/counter reset bit */
#define TIM_RESET			((uint32_t)(1<<1))
/** Timer control bit mask */
#define TIM_TCR_MASKBIT		((uint32_t)(3))

/**********************************************************************
 * Timer match control register definitions
 **********************************************************************/
/** Bit location for interrupt on MRx match, n = 0 to 3 */
#define TIM_INT_ON_MATCH(n)      	(_BIT((n * 3)))
/** Bit location for reset on MRx match, n = 0 to 3 */
#define TIM_RESET_ON_MATCH(n)    	(_BIT(((n * 3) + 1)))
/** Bit location for stop on MRx match, n = 0 to 3 */
#define TIM_STOP_ON_MATCH(n)     	(_BIT(((n * 3) + 2)))
/** Timer Match control bit mask */
#define TIM_MCR_MASKBIT			   ((uint32_t)(0x0FFF))
/** Timer Match control bit mask for specific channel*/
#define	TIM_MCR_CHANNEL_MASKBIT(n)		((uint32_t)(7<<(n*3)))

/**********************************************************************
 * Timer capture control register definitions
 **********************************************************************/
/** Bit location for CAP.n on CRx rising edge, n = 0 to 3 */
#define TIM_CAP_RISING(n)   	(_BIT((n * 3)))
/** Bit location for CAP.n on CRx falling edge, n = 0 to 3 */
#define TIM_CAP_FALLING(n)   	(_BIT(((n * 3) + 1)))
/** Bit location for CAP.n on CRx interrupt enable, n = 0 to 3 */
#define TIM_INT_ON_CAP(n)    	(_BIT(((n * 3) + 2)))
/** Mask bit for rising and falling edge bit */
#define TIM_EDGE_MASK(n)		(_SBF((n * 3), 0x03))
/** Timer capture control bit mask */
#define TIM_CCR_MASKBIT			((uint32_t)(0x3F))
/** Timer Capture control bit mask for specific channel*/
#define	TIM_CCR_CHANNEL_MASKBIT(n)		((uint32_t)(7<<(n*3)))

/**********************************************************************
 * Timer external match register definitions
 **********************************************************************/
/** Bit location for output state change of MAT.n when external match
 happens, n = 0 to 3 */
#define TIM_EM(n)    			_BIT(n)
/** Output state change of MAT.n when external match happens: no change */
#define TIM_EM_NOTHING    	((uint8_t)(0x0))
/** Output state change of MAT.n when external match happens: low */
#define TIM_EM_LOW         	((uint8_t)(0x1))
/** Output state change of MAT.n when external match happens: high */
#define TIM_EM_HIGH        	((uint8_t)(0x2))
/** Output state change of MAT.n when external match happens: toggle */
#define TIM_EM_TOGGLE      	((uint8_t)(0x3))
/** Macro for setting for the MAT.n change state bits */
#define TIM_EM_SET(n,s) 	(_SBF(((n << 1) + 4), (s & 0x03)))
/** Mask for the MAT.n change state bits */
#define TIM_EM_MASK(n) 		(_SBF(((n << 1) + 4), 0x03))
/** Timer external match bit mask */
#define TIM_EMR_MASKBIT	0x0FFF

/**********************************************************************
 * Timer Count Control Register definitions
 **********************************************************************/
/** Mask to get the Counter/timer mode bits */
#define TIM_CTCR_MODE_MASK  0x3
/** Mask to get the count input select bits */
#define TIM_CTCR_INPUT_MASK 0xC
/** Timer Count control bit mask */
#define TIM_CTCR_MASKBIT	0xF
#define TIM_COUNTER_MODE ((uint8_t)(1))

template<int timerptr>
class Timer {

public:

	enum TimerMode {
		TIMER_MODE = 0, /*!< Timer mode */
		COUNTER_RISING_MODE, /*!< Counter rising mode */
		COUNTER_FALLING_MODE, /*!< Counter falling mode */
		COUNTER_ANY_MODE /*!< Counter on both edges */
	};

	enum IntType {
		TIM_MR0_INT = 0, /*!< interrupt for Match channel 0*/
		TIM_MR1_INT = 1, /*!< interrupt for Match channel 1*/
		TIM_MR2_INT = 2, /*!< interrupt for Match channel 2*/
		TIM_MR3_INT = 3, /*!< interrupt for Match channel 3*/
		TIM_CR0_INT = 4, /*!< interrupt for Capture channel 0*/
		TIM_CR1_INT = 5 /*!< interrupt for Capture channel 1*/
	};

	enum CaptureFlags {
		CAP_RISING = 1 << 0, CAP_FALLING = 1 << 1, INT_ON_CAP = 1 << 2
	};

	enum CapturePin {
		COUNTER_INCAP0 = 0, /*!< CAPn.0 input pin for TIMERn */
		COUNTER_INCAP1, /*!< CAPn.1 input pin for TIMERn */
	};

	enum MatchFlags {
		INT_ON_MATCH = 1 << 0, RESET_ON_MATCH = 1 << 1, STOP_ON_MATCH = 1 << 2
	};

	enum ExtMatchOpt {
		TIM_EXTMATCH_NOTHING = 0, /*!< Do nothing for external output pin if match */
		TIM_EXTMATCH_LOW, /*!< Force external output pin to low if match */
		TIM_EXTMATCH_HIGH, /*!< Force external output pin to high if match */
		TIM_EXTMATCH_TOGGLE /*!< Toggle external output pin if match */
	};

	static void enableTimer(uint32_t prescaleValue) {
		initPower();
		resetTimer();

		TIMx->PR = prescaleValue - 1;

		TIMx->IR = 0xFFFFFFFF;
	}

	static void enableCounter(CapturePin cap, TimerMode mode =
			TimerMode::COUNTER_RISING_MODE) {
		initPower();
		resetTimer();

		TIMx->CTCR &= ~TIM_CTCR_INPUT_MASK;
		if (cap == COUNTER_INCAP1)
			TIMx->CTCR |= _BIT(2);

		// Clear interrupt pending
		TIMx->IR = 0xFFFFFFFF;
	}

	static void shutdown() {
		TIMx->TCR = 0x00;

		if (TIMx == LPC_TIM0)
			CLKPwr::setClkPower(CLKPwr::PType::PCTIM0, false);

		else if (TIMx == LPC_TIM1)
			CLKPwr::setClkPower(CLKPwr::PType::PCTIM1, false);

		else if (TIMx == LPC_TIM2)
			CLKPwr::setClkPower(CLKPwr::PType::PCTIM2, false);

		else if (TIMx == LPC_TIM3)
			CLKPwr::setClkPower(CLKPwr::PType::PCTIM3, false);

	}

	static void enable(bool NewState = true) {
		if (NewState) {
			TIMx->TCR |= TIM_ENABLE;
		} else {
			TIMx->TCR &= ~TIM_ENABLE;
		}
	}

	static void disable() {
		enable(false);
	}

	static void resetCounter() {
		TIMx->TCR |= TIM_RESET;
		TIMx->TCR &= ~TIM_RESET;
	}

	static void configureMatch(uint8_t channel, uint32_t matchValue,
			MatchFlags matchFlags, ExtMatchOpt extMatch) {

		switch (channel) {
		case 0:
			TIMx->MR0 = matchValue;
			break;
		case 1:
			TIMx->MR1 = matchValue;
			break;
		case 2:
			TIMx->MR2 = matchValue;
			break;
		case 3:
			TIMx->MR3 = matchValue;
			break;
		}

		//interrupt on MRn
		TIMx->MCR &= ~TIM_MCR_CHANNEL_MASKBIT(channel);

		if (matchFlags & MatchFlags::INT_ON_MATCH)
			TIMx->MCR |= TIM_INT_ON_MATCH(channel);

		//reset on MRn
		if (matchFlags & MatchFlags::RESET_ON_MATCH)
			TIMx->MCR |= TIM_RESET_ON_MATCH(channel);

		//stop on MRn
		if (matchFlags & MatchFlags::STOP_ON_MATCH)
			TIMx->MCR |= TIM_STOP_ON_MATCH(channel);

		// match output type

		TIMx->EMR &= ~TIM_EM_MASK(channel);
		TIMx->EMR |= TIM_EM_SET(channel, extMatch);
	}

	static void updateMatchValue(uint8_t MatchChannel, uint32_t MatchValue) {

		switch (MatchChannel) {
		case 0:
			TIMx->MR0 = MatchValue;
			break;
		case 1:
			TIMx->MR1 = MatchValue;
			break;
		case 2:
			TIMx->MR2 = MatchValue;
			break;
		case 3:
			TIMx->MR3 = MatchValue;
			break;
		}

	}

	static void configureCapture(uint8_t channel, CaptureFlags flags) {

		TIMx->CCR &= ~TIM_CCR_CHANNEL_MASKBIT(channel);

		if (flags & CAP_RISING)
			TIMx->CCR |= TIM_CAP_RISING(channel);

		if (flags & CAP_FALLING)
			TIMx->CCR |= TIM_CAP_FALLING(channel);

		if (flags & INT_ON_CAP)
			TIMx->CCR |= TIM_INT_ON_CAP(channel);
	}

	static uint32_t getCaptureValue(uint8_t channel) {

		if (channel == 0)
			return TIMx->CR0;
		else
			return TIMx->CR1;
	}

	static void clearIntCapturePending(IntType IntFlag) {
		TIMx->IR = (1 << (4 + IntFlag));
	}

	static void clearIntPending(IntType IntFlag) {

		TIMx->IR = TIM_IR_CLR(IntFlag);
	}

	static bool getIntCaptureStatus(IntType IntFlag) {
		uint8_t temp;

		return (TIMx->IR) & (1 << (4 + IntFlag));
	}

	static bool getIntStatus(IntType IntFlag) {
		uint8_t temp;

		return (TIMx->IR) & TIM_IR_CLR(IntFlag);
	}

private:

	static void initPower() {
		if (TIMx == LPC_TIM0) {
			CLKPwr::setClkPower(CLKPwr::PType::PCTIM0, true);
			//PCLK_Timer0 = CCLK/4
			CLKPwr::setClkDiv(CLKPwr::ClkType::TIMER0, CLKPwr::ClkDiv::DIV_1);
		} else if (TIMx == LPC_TIM1) {
			CLKPwr::setClkPower(CLKPwr::PType::PCTIM1, true);
			CLKPwr::setClkDiv(CLKPwr::ClkType::TIMER1, CLKPwr::ClkDiv::DIV_1);
		}
		else if (TIMx == LPC_TIM2) {
			CLKPwr::setClkPower(CLKPwr::PType::PCTIM2, true);
			CLKPwr::setClkDiv(CLKPwr::ClkType::TIMER2, CLKPwr::ClkDiv::DIV_1);

		} else if (TIMx == LPC_TIM3) {
			CLKPwr::setClkPower(CLKPwr::PType::PCTIM3, true);
			CLKPwr::setClkDiv(CLKPwr::ClkType::TIMER3, CLKPwr::ClkDiv::DIV_1);

		}
	}

	static void resetTimer() {
		TIMx->CTCR &= ~TIM_CTCR_MODE_MASK;
		TIMx->CTCR |= TimerMode::TIMER_MODE;

		TIMx->TC = 0;
		TIMx->PC = 0;
		TIMx->PR = 0;
		TIMx->TCR |= (1 << 1); //Reset Counter
		TIMx->TCR &= ~(1 << 1); //release reset
	}

};

typedef Timer<(int)LPC_TIM0> Timer0;
typedef Timer<(int)LPC_TIM1> Timer1;
typedef Timer<(int)LPC_TIM2> Timer2;
typedef Timer<(int)LPC_TIM3> Timer3;

}

}

#endif /* TIMER_HPP_ */
