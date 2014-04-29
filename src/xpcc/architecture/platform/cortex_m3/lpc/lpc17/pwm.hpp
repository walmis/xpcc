/*
 * pwm.hpp
 *
 *  Created on: Oct 23, 2013
 *      Author: walmis
 */

#ifndef PWM_HPP_
#define PWM_HPP_

#include <xpcc/architecture.hpp>

namespace xpcc {
namespace lpc17 {

/* --------------------- BIT DEFINITIONS -------------------------------------- */
/**********************************************************************
* IR register definitions
**********************************************************************/
/** Interrupt flag for PWM match channel for 6 channel */
#define PWM_IR_PWMMRn(n)    	((uint32_t)((n<4)?(1<<n):(1<<(n+4))))
/** Interrupt flag for capture input */
#define PWM_IR_PWMCAPn(n)		((uint32_t)(1<<(n+4)))
/**  IR register mask */
#define PWM_IR_BITMASK			((uint32_t)(0x0000073F))

/**********************************************************************
* TCR register definitions
**********************************************************************/
/** TCR register mask */
#define PWM_TCR_BITMASK				((uint32_t)(0x0000000B))
#define PWM_TCR_COUNTER_ENABLE      ((uint32_t)(1<<0)) /*!< PWM Counter Enable */
#define PWM_TCR_COUNTER_RESET       ((uint32_t)(1<<1)) /*!< PWM Counter Reset */
#define PWM_TCR_PWM_ENABLE          ((uint32_t)(1<<3)) /*!< PWM Enable */

/**********************************************************************
* CTCR register definitions
**********************************************************************/
/** CTCR register mask */
#define PWM_CTCR_BITMASK			((uint32_t)(0x0000000F))
/** PWM Counter-Timer Mode */
#define PWM_CTCR_MODE(n)        	((uint32_t)(n&0x03))
/** PWM Capture input select */
#define PWM_CTCR_SELECT_INPUT(n)	((uint32_t)((n&0x03)<<2))

/**********************************************************************
* MCR register definitions
**********************************************************************/
/** MCR register mask */
#define PWM_MCR_BITMASK				((uint32_t)(0x001FFFFF))
/** generate a PWM interrupt when a MATCHn occurs */
#define PWM_MCR_INT_ON_MATCH(n)     ((uint32_t)(1<<(((n&0x7)<<1)+(n&0x07))))
/** reset the PWM when a MATCHn occurs */
#define PWM_MCR_RESET_ON_MATCH(n)   ((uint32_t)(1<<(((n&0x7)<<1)+(n&0x07)+1)))
/** stop the PWM when a MATCHn occurs */
#define PWM_MCR_STOP_ON_MATCH(n)    ((uint32_t)(1<<(((n&0x7)<<1)+(n&0x07)+2)))

/**********************************************************************
* CCR register definitions
**********************************************************************/
/** CCR register mask */
#define PWM_CCR_BITMASK				((uint32_t)(0x0000003F))
/** PCAPn is rising edge sensitive */
#define PWM_CCR_CAP_RISING(n) 	 	((uint32_t)(1<<(((n&0x2)<<1)+(n&0x1))))
/** PCAPn is falling edge sensitive */
#define PWM_CCR_CAP_FALLING(n) 		((uint32_t)(1<<(((n&0x2)<<1)+(n&0x1)+1)))
/** PWM interrupt is generated on a PCAP event */
#define PWM_CCR_INT_ON_CAP(n)  		((uint32_t)(1<<(((n&0x2)<<1)+(n&0x1)+2)))

/**********************************************************************
* PCR register definitions
**********************************************************************/
/** PCR register mask */
#define PWM_PCR_BITMASK			(uint32_t)0x00007E7C
/** PWM output n is a single edge controlled output */
#define PWM_PCR_PWMSELn(n)   	((uint32_t)(((n&0x7)<2) ? 0 : (1<<n)))
/** enable PWM output n */
#define PWM_PCR_PWMENAn(n)   	((uint32_t)(((n&0x7)<1) ? 0 : (1<<(n+8))))

/**********************************************************************
* LER register definitions
**********************************************************************/
/** LER register mask*/
#define PWM_LER_BITMASK				((uint32_t)(0x0000007F))
/** PWM MATCHn register update control */
#define PWM_LER_EN_MATCHn_LATCH(n)   ((uint32_t)((n<7) ? (1<<n) : 0))

class PWM {

public:
	/** @brief PWM update type */
	enum UpdateType {
		PWM_MATCH_UPDATE_NOW,
		PWM_MATCH_UPDATE_NEXT_RST
	};

	typedef enum {
	    PWM_CHANNEL_SINGLE_EDGE,	/*!< PWM Channel Single edge mode */
	    PWM_CHANNEL_DUAL_EDGE		/*!< PWM Channel Dual edge mode */
	} PWM_CHANNEL_EDGE_OPT;

	enum CaptureFlags {
		RISING_EDGE = 1<<0,
		FALLING_EDGE = 1<<1,
		INT_ON_CAPTURE = 1<<2
	};

	enum CounterEdgeOption {
	    PWM_COUNTER_RISING = 1,		/*!< Rising edge mode */
	    PWM_COUNTER_FALLING = 2,	/*!< Falling edge mode */
	    PWM_COUNTER_ANY = 3			/*!< Both rising and falling mode */
	};

	enum CounterInputSelect {
			PWM_COUNTER_PCAP1_0 = 0,		/*!< PWM Counter input selected is PCAP1.0 pin */
			PWM_COUNTER_PCAP1_1			/*!< PWM counter input selected is CAP1.1 pin */
	};

	enum PWMMode {
		PWM_MODE_TIMER = 0,		/*!< PWM using Timer mode */
		PWM_MODE_COUNTER		/*!< PWM using Counter mode */
	};

	enum IntType
	{
		PWM_INTSTAT_MR0 = PWM_IR_PWMMRn(0), 	/**< Interrupt flag for PWM match channel 0 */
		PWM_INTSTAT_MR1 = PWM_IR_PWMMRn(1),		/**< Interrupt flag for PWM match channel 1 */
		PWM_INTSTAT_MR2 = PWM_IR_PWMMRn(2),		/**< Interrupt flag for PWM match channel 2 */
		PWM_INTSTAT_MR3 = PWM_IR_PWMMRn(3),		/**< Interrupt flag for PWM match channel 3 */
		PWM_INTSTAT_CAP0 = PWM_IR_PWMCAPn(0),	/**< Interrupt flag for capture input 0 */
		PWM_INTSTAT_CAP1 = PWM_IR_PWMCAPn(1),	/**< Interrupt flag for capture input 1 */
		PWM_INTSTAT_MR4 = PWM_IR_PWMMRn(4),		/**< Interrupt flag for PWM match channel 4 */
		PWM_INTSTAT_MR6 = PWM_IR_PWMMRn(5),		/**< Interrupt flag for PWM match channel 5 */
		PWM_INTSTAT_MR5 = PWM_IR_PWMMRn(6)		/**< Interrupt flag for PWM match channel 6 */
	};

	static bool getIntStatus(IntType IntFlag) {
		return ((LPC_PWM1 ->IR & IntFlag) ? true : false);
	}

	/*********************************************************************//**
	 * @brief 		Clear specified PWM Interrupt pending
	 * @param[in]	IntFlag: PWM interrupt flag, should be:
	 * 				- PWM_INTSTAT_MR0: Interrupt flag for PWM match channel 0
	 * 				- PWM_INTSTAT_MR1: Interrupt flag for PWM match channel 1
	 * 				- PWM_INTSTAT_MR2: Interrupt flag for PWM match channel 2
	 * 				- PWM_INTSTAT_MR3: Interrupt flag for PWM match channel 3
	 * 				- PWM_INTSTAT_MR4: Interrupt flag for PWM match channel 4
	 * 				- PWM_INTSTAT_MR5: Interrupt flag for PWM match channel 5
	 * 				- PWM_INTSTAT_MR6: Interrupt flag for PWM match channel 6
	 * 				- PWM_INTSTAT_CAP0: Interrupt flag for capture input 0
	 * 				- PWM_INTSTAT_CAP1: Interrupt flag for capture input 1
	 * @return 		None
	 **********************************************************************/
	static void clearIntPending(IntType IntFlag) {

		LPC_PWM1 ->IR = IntFlag;
	}


	static void initTimer(uint32_t prescale) {
		init();

		LPC_PWM1 ->PR = prescale - 1;
	}

	static void initCounter(CounterEdgeOption copt, CounterInputSelect sel) {

		init();

		LPC_PWM1 ->CTCR |= (PWM_CTCR_MODE(
				(uint32_t) copt) ) | (PWM_CTCR_SELECT_INPUT(
				(uint32_t) sel) );

	}

	/*********************************************************************//**
	 * @brief		De-initializes the PWM peripheral registers to their
	 *                  default reset values.
	 * @param[in]	LPC_PWM1	PWM peripheral selected, should be LPC_PWM1
	 * @return 		None
	 **********************************************************************/
	static void shutdown()
	{
		// Disable PWM control (timer, counter and PWM)
		LPC_PWM1->TCR = 0x00;

		CLKPwr::setClkPower(CLKPwr::PType::PCPWM1, false);
		NVIC_DisableIRQ(PWM1_IRQn);
	}

	/*********************************************************************//**
	 * @brief	 	Enable/Disable PWM peripheral
	 * @param[in]	LPC_PWM1	PWM peripheral selected, should be LPC_PWM1
	 * @param[in]	NewState	New State of this function, should be:
	 * 							- ENABLE: Enable PWM peripheral
	 * 							- DISABLE: Disable PWM peripheral
	 * @return 		None
	 **********************************************************************/
	static void enable(bool NewState = true) {

		if (NewState == true) {
			LPC_PWM1 ->TCR |= PWM_TCR_PWM_ENABLE;
		} else {
			LPC_PWM1 ->TCR &= (~PWM_TCR_PWM_ENABLE) & PWM_TCR_BITMASK;
		}

		counterEnable(NewState);
	}

	/*********************************************************************//**
	 * @brief 		Enable/Disable Counter in PWM peripheral
	 * @param[in]	LPC_PWM1	PWM peripheral selected, should be LPC_PWM1
	 * @param[in]	NewState New State of this function, should be:
	 * 							- ENABLE: Enable Counter in PWM peripheral
	 * 							- DISABLE: Disable Counter in PWM peripheral
	 * @return 		None
	 **********************************************************************/
	static void counterEnable(bool NewState = true) {

		if (NewState == true) {
			LPC_PWM1 ->TCR |= PWM_TCR_COUNTER_ENABLE;
		} else {
			LPC_PWM1 ->TCR &= (~PWM_TCR_COUNTER_ENABLE) & PWM_TCR_BITMASK;
		}
	}

	/*********************************************************************//**
	 * @brief 		Reset Counter in PWM peripheral
	 * @param[in]	LPC_PWM1	PWM peripheral selected, should be LPC_PWM1
	 * @return 		None
	 **********************************************************************/
	static void resetCounter()
	{

		LPC_PWM1->TCR |= PWM_TCR_COUNTER_RESET;
		LPC_PWM1->TCR &= (~PWM_TCR_COUNTER_RESET) & PWM_TCR_BITMASK;
	}


	enum MatchFlags {
		INT_ON_MATCH = 1<<0,
		RESET_ON_MATCH = 1<<1,
		STOP_ON_MATCH = 1<<2
	};
	/*********************************************************************//**
	 * @brief 		Configures match for PWM peripheral
	 * @param[in]	LPC_PWM1	PWM peripheral selected, should be LPC_PWM1
	 * @param[in]   PWM_MatchConfigStruct	Pointer to a PWM_MATCHCFG_Type structure
	 *                    that contains the configuration information for the
	 *                    specified PWM match function.
	 * @return 		None
	 **********************************************************************/
	static void configureMatch(uint8_t channel, MatchFlags flags) {

		//interrupt on MRn
		if (flags & INT_ON_MATCH) {
			LPC_PWM1 ->MCR |= PWM_MCR_INT_ON_MATCH(
					channel);
		} else {
			LPC_PWM1 ->MCR &= (~PWM_MCR_INT_ON_MATCH(
					channel)) & PWM_MCR_BITMASK;
		}

		//reset on MRn
		if (flags & RESET_ON_MATCH) {
			LPC_PWM1 ->MCR |= PWM_MCR_RESET_ON_MATCH(
					channel);
		} else {
			LPC_PWM1 ->MCR &= (~PWM_MCR_RESET_ON_MATCH(
					channel)) & PWM_MCR_BITMASK;
		}

		//stop on MRn
		if (flags & STOP_ON_MATCH) {
			LPC_PWM1 ->MCR |= PWM_MCR_STOP_ON_MATCH(
					channel);
		} else {
			LPC_PWM1 ->MCR &= (~PWM_MCR_STOP_ON_MATCH(
					channel)) & PWM_MCR_BITMASK;
		}
	}



	/*********************************************************************//**
	 * @brief 		Configures capture input for PWM peripheral
	 * @param[in]	LPC_PWM1	PWM peripheral selected, should be LPC_PWM1
	 * @param[in]   PWM_CaptureConfigStruct	Pointer to a PWM_CAPTURECFG_Type structure
	 *                    that contains the configuration information for the
	 *                    specified PWM capture input function.
	 * @return 		None
	 **********************************************************************/
	static void configureCapture(uint8_t channel, CaptureFlags flags) {

		if (flags & RISING_EDGE) {
			LPC_PWM1 ->CCR |= PWM_CCR_CAP_RISING(
					channel);
		} else {
			LPC_PWM1 ->CCR &= (~PWM_CCR_CAP_RISING(
					channel)) & PWM_CCR_BITMASK;
		}

		if (flags & FALLING_EDGE) {
			LPC_PWM1 ->CCR |= PWM_CCR_CAP_FALLING(
					channel);
		} else {
			LPC_PWM1 ->CCR &= (~PWM_CCR_CAP_FALLING(
					channel)) & PWM_CCR_BITMASK;
		}

		if (flags & INT_ON_CAPTURE) {
			LPC_PWM1 ->CCR |= PWM_CCR_INT_ON_CAP(
					channel);
		} else {
			LPC_PWM1 ->CCR &= (~PWM_CCR_INT_ON_CAP(
					channel)) & PWM_CCR_BITMASK;
		}
	}

	/*********************************************************************//**
	 * @brief 		Read value of capture register PWM peripheral
	 * @param[in]	LPC_PWM1	PWM peripheral selected, should be LPC_PWM1
	 * @param[in]	CaptureChannel: capture channel number, should be in
	 * 				range 0 to 1
	 * @return 		Value of capture register
	 **********************************************************************/
	static uint32_t getCaptureValue(uint8_t CaptureChannel) {

		switch (CaptureChannel) {
		case 0:
			return LPC_PWM1 ->CR0;

		case 1:
			return LPC_PWM1 ->CR1;

		default:
			return (0);
		}
	}



	/********************************************************************//**
	 * @brief 		Update value for each PWM channel with update type option
	 * @param[in]	LPC_PWM1	PWM peripheral selected, should be LPC_PWM1
	 * @param[in]	MatchChannel Match channel
	 * @param[in]	MatchValue Match value
	 * @param[in]	UpdateType Type of Update, should be:
	 * 				- PWM_MATCH_UPDATE_NOW: The update value will be updated for
	 * 					this channel immediately
	 * 				- PWM_MATCH_UPDATE_NEXT_RST: The update value will be updated for
	 * 					this channel on next reset by a PWM Match event.
	 * @return		None
	 *********************************************************************/
	static void matchUpdate(uint8_t MatchChannel, uint32_t MatchValue,
			UpdateType UpdateType = PWM_MATCH_UPDATE_NOW) {

		updateMatchRegister(MatchChannel, MatchValue);

		// Write Latch register
		LPC_PWM1 ->LER |= PWM_LER_EN_MATCHn_LATCH(MatchChannel);

		// In case of update now
		if (UpdateType == PWM_MATCH_UPDATE_NOW) {
			LPC_PWM1 ->TCR |= PWM_TCR_COUNTER_RESET;
			LPC_PWM1 ->TCR &= (~PWM_TCR_COUNTER_RESET) & PWM_TCR_BITMASK;
		}
	}

	class MatchUpdate {
	public:
		MatchUpdate() {
			latchValue = 0;
		}

		MatchUpdate* set(uint8_t channel, uint32_t value) {
			PWM::updateMatchRegister(channel, value);
			latchValue |= 1<<channel;
			return this;
		}

		void commit(UpdateType type = PWM_MATCH_UPDATE_NOW) {
			LPC_PWM1 ->LER = latchValue;

			if (type == PWM_MATCH_UPDATE_NOW) {
				LPC_PWM1 ->TCR |= PWM_TCR_COUNTER_RESET;
				LPC_PWM1 ->TCR &= (~PWM_TCR_COUNTER_RESET) & PWM_TCR_BITMASK;
			}
		}
		uint8_t latchValue;
	};

	static MatchUpdate multiMatchUpdate() {
		MatchUpdate m;
		return m;
	}


	/********************************************************************//**
	 * @brief 		Configure Edge mode for each PWM channel
	 * @param[in]	LPC_PWM1	PWM peripheral selected, should be LPC_PWM1
	 * @param[in]	PWMChannel PWM channel, should be in range from 2 to 6
	 * @param[in]	ModeOption PWM mode option, should be:
	 * 				- PWM_CHANNEL_SINGLE_EDGE: Single Edge mode
	 * 				- PWM_CHANNEL_DUAL_EDGE: Dual Edge mode
	 * @return 		None
	 * Note: PWM Channel 1 can not be selected for mode option
	 *********************************************************************/
	static void channelConfig(uint8_t PWMChannel, uint8_t ModeOption) {

		// Single edge mode
		if (ModeOption == PWM_CHANNEL_SINGLE_EDGE) {
			LPC_PWM1 ->PCR &= (~PWM_PCR_PWMSELn(PWMChannel)) & PWM_PCR_BITMASK;
		}
		// Double edge mode
		else if (PWM_CHANNEL_DUAL_EDGE) {
			LPC_PWM1 ->PCR |= PWM_PCR_PWMSELn(PWMChannel);
		}
	}

	/********************************************************************//**
	 * @brief 		Enable/Disable PWM channel output
	 * @param[in]	LPC_PWM1	PWM peripheral selected, should be LPC_PWM1
	 * @param[in]	PWMChannel PWM channel, should be in range from 1 to 6
	 * @param[in]	NewState New State of this function, should be:
	 * 				- ENABLE: Enable this PWM channel output
	 * 				- DISABLE: Disable this PWM channel output
	 * @return		None
	 *********************************************************************/
	static void channelEnable(uint8_t PWMChannel, bool NewState = true) {

		if (NewState == true) {
			LPC_PWM1 ->PCR |= PWM_PCR_PWMENAn(PWMChannel);
		} else {
			LPC_PWM1 ->PCR &= (~PWM_PCR_PWMENAn(PWMChannel)) & PWM_PCR_BITMASK;
		}
	}

	friend class MatchUpdate;

private:
	static void init() {

		CLKPwr::setClkPower(CLKPwr::PType::PCPWM1, true);
		CLKPwr::setClkDiv(CLKPwr::ClkType::PWM1, CLKPwr::ClkDiv::DIV_1);

		// Clear all interrupts pending
		LPC_PWM1 ->IR = 0xFF & PWM_IR_BITMASK;
		LPC_PWM1 ->TCR = 0x00;
		LPC_PWM1 ->CTCR = 0x00;
		LPC_PWM1 ->MCR = 0x00;
		LPC_PWM1 ->CCR = 0x00;
		LPC_PWM1 ->PCR = 0x00;
		LPC_PWM1 ->LER = 0x00;

		NVIC_EnableIRQ(PWM1_IRQn);
	}

	static ALWAYS_INLINE
	void updateMatchRegister(uint8_t MatchChannel, uint32_t MatchValue) {

		switch (MatchChannel) {
		case 0:
			LPC_PWM1 ->MR0 = MatchValue;
			break;

		case 1:
			LPC_PWM1 ->MR1 = MatchValue;
			break;

		case 2:
			LPC_PWM1 ->MR2 = MatchValue;
			break;

		case 3:
			LPC_PWM1 ->MR3 = MatchValue;
			break;

		case 4:
			LPC_PWM1 ->MR4 = MatchValue;
			break;

		case 5:
			LPC_PWM1 ->MR5 = MatchValue;
			break;

		case 6:
			LPC_PWM1 ->MR6 = MatchValue;
			break;
		}
	}
};

inline PWM::MatchFlags operator | (PWM::MatchFlags lhs, PWM::MatchFlags rhs)
{
    return (PWM::MatchFlags)( (int)lhs | (int)rhs );
}

inline PWM::CaptureFlags operator | (PWM::CaptureFlags lhs, PWM::CaptureFlags rhs)
{
    return (PWM::CaptureFlags)( (int)lhs | (int)rhs );
}


}
}

#endif /* PWM_HPP_ */
