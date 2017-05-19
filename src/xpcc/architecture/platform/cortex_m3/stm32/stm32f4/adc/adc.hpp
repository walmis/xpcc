// coding: utf-8
/* Copyright (c) 2012-2013, Roboterclub Aachen e.V.
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------
//%#
//%#
//%% set channels = [0,1,2,3,4,5,6,7,8,10,11,12,13]
//%% if target is stm32f2 or target is stm32f4
//	%% do channels.extend([9,14,15,16,17,18])
//%% elif target is stm32f1
//	%% if id == 1
//		%% do channels.extend([16,17])
//	%% endif
//	%% if id < 3
//		%% do channels.extend([9,14,15])
//	%% endif
//%% endif
//%% set channels = channels|sort
//%#
//%#
//%% if target is stm32f2 or target is stm32f4 or (target is stm32f1 and id == 1)
//	%% set temperature_available = true
//%% else
//	%% set temperature_available = false
//%% endif
//%#
/** \file adc_{{ id }}.hpp
 *
 *	The documentation is compiled for STM32F4XX.
 *
 *  @date	03.04.2012
 *  @author	David Hebbeker
 */

#pragma once

#include <stdint.h>
#include <xpcc/architecture/utils.hpp>
#include "../../device.hpp"

namespace xpcc {

namespace stm32 {

/**
 * Analog/Digital-Converter module (ADCx).
 * 
 * The 12-bit ADC is a successive approximation analog-to-digital
 * converter. It has up to 18 multiplexed channels allowing it measure
 * signals from 16 external and two internal sources.
 * The result of the ADC is stored in a left-aligned or right-aligned
 * 16-bit data register.
 * 
 * This API is designed for the internal ADCs of STM32F4XX, 
 * STM32F10X_LD, STM32F10X_LD_VL, STM32F10X_MD, STM32F10X_MD_VL, 
 * STM32F10X_HD, STM32F10X_HD_VL, STM32F10X_XL and STM32F10X_CL.
 * 
 * \author	Stephan Kugelmann, David Hebbeker
 * \ingroup	stm32
 */

#define ADCx (reinterpret_cast<ADC_TypeDef*>(adcptr))

template<int adcptr>
class Adc {
public:

public:
	/**
	 * Channels, which can be used with this ADC.
	 * 
	 * You can specify the channel by using a pin-name, like PIN_C0, an 
	 * internal sensor, like TEMPERATURE_SENSOR or just the plain 
	 * channel number, like CHANNEL_0. 
	 */
	enum class Channel
		: uint8_t	// TODO: What is the best type?
		{

			Channel_1 = 0,
		Channel_2,
		Channel_3,
		Channel_4,
		Channel_5,
		Channel_6,
		Channel_7,
		Channel_8,
		Channel_9,
		Channel_10,
		Channel_11,
		Channel_12,
		Channel_13,
		Channel_14,
		Channel_15,
#if defined(STM32F2XX) || defined(STM32F4XX)
		BatDiv2 = 18, /// Half the V_BAT voltage.
#endif

#if defined(STM32F2XX) || defined(STM32F4XX)
		/** Measure the ambient temperature of the device.
		 * 
		 * \li Supported temperature range: -40 to 125 C
		 * \li Precision: +-1.5 C
		 * 
		 * @see Reference manual (i.e. RM0090) for the formula for the
		 * 	calculation of the actual temperature.
		 * @note The TSVREFE bit must be set to enable the conversion of 
		 * 	this internal channel.
		 */
		TemperatureSensor = 16,

		/** Internal reference voltage.
		 * 
		 * @note The TSVREFE bit must be set to enable the conversion of 
		 * 	this internal channel.
		 */
		InternalReference = 17,
#endif
	};

	/**
	 * Programmable prescaler to divide the APB2 clock frequency, which 
	 * is used fot the analog circuitry (not the digital interface which
	 * is used for registers). 
	 */
	enum class Prescaler
		: uint8_t
		{
			Div2 = 0b00,	//!< PCLK2 divided by 2
		Div4 = 0b01,	//!< PCLK2 divided by 4
		Div6 = 0b10,	//!< PCLK2 divided by 6
		Div8 = 0b11	//!< PCLK2 divided by 8
	};

	/**
	 * Sampling time of the input voltage.
	 * 
	 * Total conversion time is T_con = Sampling time + 12 cycles
	 */
	enum class SampleTime
		: uint8_t
		{
#if defined(STM32F2XX) || defined(STM32F4XX)
		Cycles3 = 0b000,	//!< 3 ADCCLK cycles
		Cycles15 = 0b001,	//!< 15 ADCCLK cycles
		Cycles28 = 0b010,	//!< 28 ADCCLK cycles
		Cycles56 = 0b011,	//!< 56 ADCCLK cycles
		Cycles84 = 0b100,	//!< 84 ADCCLK cycles
		Cycles112 = 0b101,	//!< 112 ADCCLK cycles
		Cycles144 = 0b110,	//!< 144 ADCCLK cycles
		Cycles480 = 0b111		//!< 480 ADCCLK cycles
#elif defined(STM32F1XX)
		Cycles2 = 0b000,	//!< 1.5 ADCCLK cycles
		Cycles8 = 0b001,//!< 7.5 ADCCLK cycles
		Cycles14 = 0b010,//!< 13.5 ADCCLK cycles
		Cycles29 = 0b011,//!< 28.5 ADCCLK cycles
		Cycles42 = 0b100,//!< 41.5 ADCCLK cycles
		Cycles56 = 0b101,//!< 55.5 ADCCLK cycles
		Cycles72 = 0b110,//!< 71.5 ADCCLK cycles
		Cycles240 = 0b111//!< 239.5 ADCCLK cycles
#endif
	};

	/**
	 * Possible interrupts.
	 * 
	 * An interrupt can be produced on the end of conversion for regular
	 * and injected groups, when the analog watchdog status bit is set 
	 * and when the overrun status bit is set. 
	 */
	enum class Interrupt
		: uint32_t
		{
			/// Analog watchdog status bit is set
		AnalogWatchdog = ADC_CR1_AWDIE,
		/// End of conversion of a regular group
		EndOfRegularConversion = ADC_CR1_EOCIE,
		/// End of conversion of an injected group
		EndOfInjectedConversion = ADC_CR1_JEOCIE,
#if defined(STM32F2XX) || defined(STM32F4XX)
		/// Overrun (if data are lost)
		Overrun = ADC_CR1_OVRIE,
#endif
	};

	enum class InterruptFlag
		: uint32_t
		{
			/// Analog watchdog status bit is set
		AnalogWatchdog = ADC_SR_AWD,
		/// End of conversion of a regular group
		EndOfRegularConversion = ADC_SR_EOC,
		/// End of conversion of an injected group
		EndOfInjectedConversion = ADC_SR_JEOC,
#if defined(STM32F2XX) || defined(STM32F4XX)
		///< Overrun (if data are lost)
		Overrun = ADC_SR_OVR,
		/// All InterruptFlags
		All = ADC_SR_AWD | ADC_SR_EOC | ADC_SR_JEOC | ADC_SR_OVR,
#else
	/// All InterruptFlags
	All = ADC_SR_AWD | ADC_SR_EOC | ADC_SR_JEOC,
#endif
};

public:
/**
 * Initialize and enable the A/D converter.
 *
 * Enables the ADC clock and switches on the ADC. The ADC clock
 * prescaler will be set as well.
 *
 * The ADC clock must not exceed 14 MHz. Default is a prescaler
 * of 8 which gives a 9 Mhz ADC clock at 72 MHz APB2 clock.
 *
 * @param prescaler
 * 		The prescaler specifies by which factor the
 * 		APB2 clock (72 MHz) will be divided.
 */
static inline void
initialize(Prescaler pre = Prescaler::Div8);

// TODO
//static void
//calibrate();

/**
 * Select the frequency of the clock to the ADC. The clock is common
 * for all the ADCs (ADC1, ADC2, ADC3) and all channels.
 *
 * @pre The ADC clock must be started and the ADC switched on with
 * 	initialize()
 *
 * @param prescaler
 * 		The prescaler specifies by which factor the system clock
 * 		will be divided.
 */
static inline void setPrescaler(const Prescaler prescaler = Prescaler::Div8) {
#if defined(STM32F2XX) || defined(STM32F4XX)
	ADC->CCR &= ~(0b11 << 17);								// clear prescaler
	ADC->CCR |= static_cast<uint32_t>(prescaler) << 17;		// set prescaler
#elif defined(STM32F1XX)
			RCC->CFGR &= ~(0b11 << 14);						// clear prescaler
			RCC->CFGR |= static_cast<uint32_t>(prescaler) << 14;// set prescaler
#endif
}

#if defined(STM32F2XX) || defined(STM32F4XX)
/// Switch on temperature- and V_REF measurement.
static inline void enableTemperatureRefVMeasurement() {
#if defined(STM32F2XX) || defined(STM32F4XX)
	ADC->CCR |= ADC_CCR_TSVREFE;
#elif defined(STM32F1XX)
	ADCx->CR2 |= ADC_CR2_TSVREFE;
#endif
}

/// Switch on temperature- and V_REF measurement.
static inline void disableTemperatureRefVMeasurement() {
#if defined(STM32F2XX) || defined(STM32F4XX)
	ADC->CCR &= ~ADC_CCR_TSVREFE;
#elif defined(STM32F1XX)
	ADCx->CR2 &= ~ADC_CR2_TSVREFE;
#endif
}
#endif

static inline void dmaRequestAfterLastTransfer(bool enable) {
	if(enable) {
		ADCx->CR2 |= (uint32_t)ADC_CR2_DDS;
	} else {
		ADCx->CR2 &= (uint32_t)(~ADC_CR2_DDS);
	}
}

static inline void enableVBatMeasurement() {
	ADC->CCR |= ADC_CCR_VBATE;
}

static inline void disableVBatMeasurement() {
	ADC->CCR &= ~ADC_CCR_VBATE;
}

/**
 * Change the presentation of the ADC conversion result.
 *
 * @param enable
 * 		Set to \c true to left adjust the result.
 *		Otherwise, the result is right adjusted.
 *
 * @pre The ADC clock must be started and the ADC switched on with
 * 		initialize()
 */
static inline void setLeftAdjustResult(const bool enable) {
	if (enable) {
		ADCx->CR2 |= ADC_CR2_ALIGN;
	} else {
		ADCx->CR2 &= ~ADC_CR2_ALIGN;
	}
}

/**
 * Analog channel selection.
 *
 * This not for scan mode. The number of channels will be set to 1,
 * the channel selected and the corresponding pin will be set to
 * analog input.
 * If the the channel is modified during a conversion, the current
 * conversion is reset and a new start pulse is sent to the ADC to
 * convert the new chosen channnel / group of channels.
 *
 *
 * @param channel		The channel which shall be read.
 * @param sampleTime	The sample time to sample the input voltage.
 *
 * @pre The ADC clock must be started and the ADC switched on with
 * 		initialize()
 */
static inline bool setChannel(const Channel channel,
		const SampleTime sampleTime = static_cast<SampleTime>(0b000)) {
	if (uint32_t(channel) >= 18)
		return false;
	// clear number of conversions in the sequence
	// and set number of conversions to 1
	ADCx->SQR1 = 0;
	ADCx->SQR2 = 0;
	ADCx->SQR3 = uint32_t(channel) & 0x1f;

	setSampleTime(channel, sampleTime);
	return true;
}

static inline bool setChannel(uint8_t index, const Channel channel,
		const SampleTime sampleTime = static_cast<SampleTime>(0b000)) {
	if (uint32_t(channel) >= 18)
		return false;

	if (index < 6) {
		ADCx->SQR3 &= ~(0x1f << (index * 5));
		ADCx->SQR3 |= (uint32_t(channel) & 0x1f) << (index * 5);
	} else if (index < 12) {
		ADCx->SQR2 &= ~(0x1f << ((index - 6) * 5));
		ADCx->SQR2 |= (uint32_t(channel) & 0x1f) << ((index - 6) * 5);
	} else {
		ADCx->SQR1 &= ~((0x1f) << ((index - 12) * 5));
		ADCx->SQR1 |= (uint32_t(channel) & 0x1f) << ((index - 12) * 5);
	}

	setSampleTime(channel, sampleTime);
	return true;
}

static inline Channel getChannel() {
	return Channel(ADCx->SQR3 & 0x1f);
}

/*
 * 	0 channel is set using setChannel
 * 	this adds additional channels [1..16] for scan mode
 */
static inline bool addChannel(const Channel channel,
		const SampleTime sampleTime) {
	// read channel count
	uint8_t channel_count = (ADCx->SQR1 & ADC_SQR1_L) >> 20;
	++channel_count;

	if (channel_count > 0x0f)
		return false; // emergency exit
	// write channel number
	if (channel_count < 6) {
		ADCx->SQR3 |= (uint32_t(channel) & 0x1f) << (channel_count * 5);
	} else if (channel_count < 12) {
		ADCx->SQR2 |= (uint32_t(channel) & 0x1f) << ((channel_count - 6) * 5);
	} else {
		ADCx->SQR1 |= (uint32_t(channel) & 0x1f) << ((channel_count - 12) * 5);
	}

	// update channel count
	ADCx->SQR1 = (ADCx->SQR1 & ~ADC_SQR1_L) | ((channel_count) << 20);

	setSampleTime(channel, sampleTime);
	return true;
}

static inline void setSampleTime(const Channel channel,
		const SampleTime sampleTime) {
	if (uint32_t(channel) < 10) {
		ADCx->SMPR2 |= uint32_t(sampleTime) << (uint32_t(channel) * 3);
	} else {
		ADCx->SMPR1 |= uint32_t(sampleTime) << ((uint32_t(channel) - 10) * 3);
	}
}

static inline void setDMAMode(bool enable) {
	ADCx->CR2 &= ~ADC_CR2_DMA;
	if(enable) {
		ADCx->CR2 |= ADC_CR2_DMA;
	}
}


static inline void setScanMode(bool enable) {
	ADCx->CR1 &= ~ADC_CR1_SCAN;
	if(enable) {
		ADCx->CR1 |= ADC_CR1_SCAN;
	}
}

/**
 * Enables free running mode
 *
 * The ADC will continously start conversions and provide the most
 * recent result in the ADC register.
 *
 * @pre The ADC clock must be started and the ADC switched on with
 * 		initialize()
 */
static inline void enableFreeRunningMode() {
	ADCx->CR2 |= ADC_CR2_CONT;	// set to continuous mode
}

/**
 * Disables free running mode
 *
 * The ADC will do only one sample and stop. The result will be in
 * the ADC register.
 *
 * @pre The ADC clock must be started and the ADC switched on with
 * 		initialize()
 */
static inline void disableFreeRunningMode() {
	ADCx->CR2 &= ~ADC_CR2_CONT;		// set to single mode
}

/**
 * Turns off the ADC and its clock.
 */
static void
shutdownAdc();
/**
 * Start a new conversion or continuous conversions.
 *
 * @pre A ADC channel must be selected with setChannel(). When using
 * 	a STM32F10x a delay of at least t_STAB after initialize() must
 * 	be waited!
 *
 * @post The result can be fetched with getValue()
 * @attention When using a STM32F10x, the application should allow a delay of t_STAB between
 * 	power up and start of conversion. Refer to Reference Manual
 * 	(RM0008) ADC_CR2_ADON.
 */
static inline void startConversion() {
	acknowledgeInterruptFlag(InterruptFlag::All);
#if defined(STM32F1XX)
	// select the SWSTART event used to trigger the start of
	// conversion of a regular group
	ADCx->CR2 |= ADC_CR2_EXTTRIG | ADC_CR2_EXTSEL_0 |
	ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_2;
#endif
	// starts single conversion for the regular group
	ADCx->CR2 |= ADC_CR2_SWSTART;
}

/**
 * @return If the conversion is finished.
 * @pre A conversion should have been started with startConversion()
 */
static inline bool isConversionFinished() {
	return (ADCx->SR & ADC_SR_EOC);
}

/**
 * @return The most recent 16bit result of the ADC conversion.
 * @pre A conversion should have been stared with startConversion()
 *
 * To have a blocking GET you might do it this way:
 * @code
 while(!isConversionFinished())
 {
 // Waiting for conversion
 }
 @endcode
 */
static inline uint16_t getValue() {
	return ADCx->DR;
}

/**
 * Enables the ADC Conversion Complete Interrupt.
 *
 * You could catch the interrupt using this example function:
 * \li for STM32F4XX: \code extern "C" void ADC_IRQHandler() \endcode
 * \li for STM32F10X: \code extern "C" void ADC1_2_IRQHandler() \endcode
 *
 * @pre The ADC clock must be started and the ADC switched on with
 * 	initialize(). Also the Interrupt Vector needs to be enabled first.
 *
 * @param priority Priority to set
 * @param interrupt The interrupt, which shall be enabled. See
 * 	Interrupt for available interrupts.
 *
 * @note ADC1 and ADC2 interrupts are mapped onto the same interrupt
 * 	vector. ADC3 interrupts are mapped onto a separate interrupt
 * 	vector.
 */
static inline void enableInterrupt(const Interrupt interrupt) {
	ADCx->CR1 |= static_cast<uint32_t>(interrupt);
}

/**
 * Disables the ADC Conversion Complete Interrupt.
 */
static inline void disableInterrupt(const Interrupt interrupt) {
	ADCx->CR1 &= ~static_cast<uint32_t>(interrupt);
}

/**
 * Returns if the th interrupt flags set.
 *
 * @pre The ADC clock must be started and the ADC switched on with
 * 	initialize()
 */
static inline InterruptFlag getInterruptFlags() {
	return static_cast<InterruptFlag>(ADCx->SR);
}

/**
 * Clears the specified interrupt flag.
 *
 * @param flag
 * 		The interrupt flag, which shall be cleared.
 *
 * @pre The ADC clock must be started and the ADC switched on with
 * 		initialize().
 */
static inline void acknowledgeInterruptFlag(const InterruptFlag flags) {
	// Flags are cleared by writing a one to the flag position.
	// Writing a zero is ignored.
	ADCx->SR = ~(uint32_t)(flags);
}

//	ENUM_CLASS_FLAG(Adc::Interrupt);
//	ENUM_CLASS_FLAG(Adc::InterruptFlag);
};

typedef Adc<(int) ADC1_BASE> Adc1;
typedef Adc<(int) ADC2_BASE> Adc2;
typedef Adc<(int) ADC3_BASE> Adc3;

} // namespace stm32

} // namespace xpcc

