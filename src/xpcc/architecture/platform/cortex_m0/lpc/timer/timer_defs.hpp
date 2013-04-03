/*
 * timer_defs.hpp
 *
 *  Created on: Apr 3, 2013
 *      Author: walmis
 */

#ifndef TIMER_DEFS_HPP_
#define TIMER_DEFS_HPP_

#define _BIT(n) (1<<n)
#define _SBF(f,v) (v<<f)

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
#define	TIM_MCR_CHANNEL_MASKBIT(n)		((uint32_t)(7<<n))
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
#define	TIM_CCR_CHANNEL_MASKBIT(n)		((uint32_t)(7<<n))

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

/**
 * @}
 */

/**
 * @}
 */


/* Public Types --------------------------------------------------------------- */
/** @defgroup TMR_Public_Types
 * @{
 */

/***********************************************************************
 * Timer device enumeration
**********************************************************************/
/** @brief interrupt  type */
typedef enum
{
	TIM_MR0_INT =0, /*!< interrupt for Match channel 0*/
	TIM_MR1_INT =1, /*!< interrupt for Match channel 1*/
	TIM_MR2_INT =2, /*!< interrupt for Match channel 2*/
	TIM_MR3_INT =3, /*!< interrupt for Match channel 3*/
	TIM_CR0_INT =4, /*!< interrupt for Capture channel 0*/
}TIM_INT_TYPE;



/** @brief Timer/Counter prescale option */
typedef enum
{
	TIM_PRESCALE_TICKVAL = 0,		/*!< Prescale in absolute value */
	TIM_PRESCALE_USVAL				/*!< Prescale in microsecond value */
} TIM_PRESCALE_OPT;

/** @brief Counter input option */
typedef enum
{
	TIM_COUNTER_INCAP0 = 0,			/*!< CAPn.0 input pin for TIMERn */
	TIM_COUNTER_INCAP1,				/*!< CAPn.1 input pin for TIMERn */
} TIM_COUNTER_INPUT_OPT;

/** @brief Timer/Counter external match option */
typedef enum
{
	TIM_EXTMATCH_NOTHING = 0,		/*!< Do nothing for external output pin if match */
	TIM_EXTMATCH_LOW,				/*!< Force external output pin to low if match */
	TIM_EXTMATCH_HIGH,				/*!< Force external output pin to high if match */
	TIM_EXTMATCH_TOGGLE				/*!< Toggle external output pin if match */
}TIM_EXTMATCH_OPT;

/** @brief Timer/counter capture mode options */
typedef enum {
	TIM_CAPTURE_NONE = 0,	/*!< No Capture */
	TIM_CAPTURE_RISING,		/*!< Rising capture mode */
	TIM_CAPTURE_FALLING,	/*!< Falling capture mode */
	TIM_CAPTURE_ANY			/*!< On both edges */
} TIM_CAP_MODE_OPT;





/** @brief Configuration structure in TIMER mode */
typedef struct
{

	uint8_t PrescaleOption;		/**< Timer Prescale option, should be:
									- TIM_PRESCALE_TICKVAL: Prescale in absolute value
									- TIM_PRESCALE_USVAL: Prescale in microsecond value
									*/
	uint8_t Reserved[3];		/**< Reserved */
	uint32_t PrescaleValue;		/**< Prescale value */
} TIM_TIMERCFG_Type;

/** @brief Configuration structure in COUNTER mode */
typedef struct {

	uint8_t CounterOption;		/**< Counter Option, should be:
								- TIM_COUNTER_INCAP0: CAPn.0 input pin for TIMERn
								- TIM_COUNTER_INCAP1: CAPn.1 input pin for TIMERn
								*/
	uint8_t CountInputSelect;
	uint8_t Reserved[2];
} TIM_COUNTERCFG_Type;

/** @brief Match channel configuration structure */
typedef struct {
	uint8_t MatchChannel;	/**< Match channel, should be in range
							from 0..3 */
	uint8_t IntOnMatch;		/**< Interrupt On match, should be:
							- ENABLE: Enable this function.
							- DISABLE: Disable this function.
							*/
	uint8_t StopOnMatch;	/**< Stop On match, should be:
							- ENABLE: Enable this function.
							- DISABLE: Disable this function.
							*/
	uint8_t ResetOnMatch;	/**< Reset On match, should be:
							- ENABLE: Enable this function.
							- DISABLE: Disable this function.
							*/

	uint8_t ExtMatchOutputType;	/**< External Match Output type, should be:
							 -	 0:	Do nothing for external output pin if match
							 -   1:	Force external output pin to low if match
							 - 	 2: Force external output pin to high if match
							 -   3: Toggle external output pin if match.
							*/
	uint8_t Reserved[3];	/** Reserved */
	uint32_t MatchValue;	/** Match value */
} TIM_MATCHCFG_Type;


/**
 * @}
 */




/**
 * @}
 */

#endif /* __LPC11XX_TIMER_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

