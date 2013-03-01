// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2012, Roboterclub Aachen e.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Roboterclub Aachen e.V. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
// ----------------------------------------------------------------------------
/** \file adc_1.hpp
 *
 *	The documentation is compiled for STM32F4XX.
 *
 *  @date	03.04.2012
 *  @author	David Hebbeker
 */

#ifndef XPCC_STM32__ADC1_HPP
#define XPCC_STM32__ADC1_HPP

#if defined(__DOXYGEN__)
	#if !defined(STM32F4XX)
		/** Symbol defined to select the platform for which the documentation is 
		 *	build.
		 */
		#define STM32F4XX	1
	#endif
#endif

#include <xpcc/architecture/platform/cortex_m3/stm32.hpp>

namespace xpcc
{
	namespace stm32
	{
		/**
		 * Analog/Digital-Converter module (ADC1).
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
		class Adc1 : public Interface
		{
		public:

			/**
			 * Channels, which can be used with this ADC.
			 * 
			 * You can specify the channel by using a pin-name, like PIN_C0, an 
			 * internal sensor, like TEMPERATURE_SENSOR or just the plain 
			 * channel number, like CHANNEL_0. 
			 */
			enum Channels
			{
				PIN_A0 = 0,
				PIN_A1 = 1,
				PIN_A2 = 2,
				PIN_A3 = 3,
				PIN_C0 = 10,
				PIN_C1 = 11,
				PIN_C2 = 12,
				PIN_C3 = 13,
				// For ADC1 and ADC2
				PIN_A4 = 4,
				PIN_A5 = 5,
				PIN_A6 = 6,
				PIN_A7 = 7,
				PIN_B0 = 8,
				PIN_B1 = 9,
				PIN_C4 = 14,
				PIN_C5 = 15,
				#if defined(STM32F2XX) || defined(STM32F4XX)				
				/** Flag to show, that the temperature and V_Ref measurements 
				 * 	are available for this ADC.
				 */ 
				#define TEMPERATURE_REFVOLTAGE_AVIALABLE (1) 
				
				/**
				 * The half V_BAT voltage.
				 */
				VBAT = 18,
				
				#elif defined(STM32F10X)				
				/** Flag to show, that the temperature and V_Ref measurements 
				 * 	are available for this ADC.
				 */ 
				#define TEMPERATURE_REFVOLTAGE_AVIALABLE (1)
				#endif
				
#ifdef TEMPERATURE_REFVOLTAGE_AVIALABLE
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
				TEMPERATURE_SENSOR = 16,

				/** Internal reference voltage.
				 * 
				 * @note The TSVREFE bit must be set to enable the conversion of 
				 * 	this internal channel.
				 */
				V_REFINT = 17,
#endif // TEMPERATURE_REFVOLTAGE_AVIALABLE
				
				CHANNEL_0 = 0,
				CHANNEL_1 = 1,
				CHANNEL_2 = 2,
				CHANNEL_3 = 3,
				CHANNEL_4 = 4,
				CHANNEL_5 = 5,
				CHANNEL_6 = 6,
				CHANNEL_7 = 7,
				CHANNEL_8 = 8,
				CHANNEL_10= 10,
				CHANNEL_11 = 11,
				CHANNEL_12 = 12,
				CHANNEL_13 = 13,
#if defined(STM32F2XX) || defined(STM32F4XX)
				CHANNEL_9 = 9,
				CHANNEL_14 = 14,
				CHANNEL_15 = 15,
				CHANNEL_16 = 16,
				CHANNEL_17 = 17,
				CHANNEL_18 = 18
#elif defined(STM32F10X)
				// For ADC1 and ADC2
				CHANNEL_9 = 9,
				CHANNEL_14 = 14,
				CHANNEL_15 = 15,
				CHANNEL_16 = 16,	//!< connected to temperature sensor
				CHANNEL_17 = 17		//!< connected to V_REFINT
				#endif
			};
			
			/**
			 * Programmable prescaler to divide the APB2 clock frequency, which 
			 * is used fot the analog circuitry (not the digital interface which
			 * is used for registers). 
			 */
			enum Prescaler
			{
				PRESCALER_2 = 0b00,	//!< PCLK2 divided by 2
				PRESCALER_4 = 0b01,	//!< PCLK2 divided by 4
				PRESCALER_6 = 0b10,	//!< PCLK2 divided by 6
				PRESCALER_8 = 0b11	//!< PCLK2 divided by 8
			};
			
			/**
			 * Sampling time of the input voltage.
			 * 
			 * Total conversion time is T_con = Sampling time + 12 cycles
			 */
			enum SampleTime
			{
#if defined(STM32F2XX) || defined(STM32F4XX)
				CYCLES_3 	= 0b000,	//!< 3 ADCCLK cycles
				CYCLES_15 	= 0b001,	//!< 15 ADCCLK cycles
				CYCLES_28 	= 0b010,	//!< 28 ADCCLK cycles
				CYCLES_56 	= 0b011,	//!< 56 ADCCLK cycles
				CYCLES_84 	= 0b100,	//!< 84 ADCCLK cycles
				CYCLES_112 	= 0b101,	//!< 112 ADCCLK cycles
				CYCLES_144 	= 0b110,	//!< 144 ADCCLK cycles
				CYCLES_480 	= 0b111		//!< 480 ADCCLK cycles
#elif defined(STM32F10X)
				CYCLES_2 	= 0b000,	//!< 1.5 ADCCLK cycles
				CYCLES_8 	= 0b001,	//!< 7.5 ADCCLK cycles
				CYCLES_14 	= 0b010,	//!< 13.5 ADCCLK cycles
				CYCLES_29 	= 0b011,	//!< 28.5 ADCCLK cycles
				CYCLES_42 	= 0b100,	//!< 41.5 ADCCLK cycles
				CYCLES_56 	= 0b101,	//!< 55.5 ADCCLK cycles
				CYCLES_72 	= 0b110,	//!< 71.5 ADCCLK cycles
				CYCLES_240 	= 0b111		//!< 239.5 ADCCLK cycles
#endif
			}; 

			/**
			 * Possible interrupts.
			 * 
			 * An interrupt can be produced on the end of conversion for regular
			 * and injected groups, when the analog watchdog status bit is set 
			 * and when the overrun status bit is set. 
			 */
			enum Interrupt
			{
				END_OF_CONVERSION_REGULAR	= ADC_SR_EOC,	//!< End of conversion of a regular group
				END_OF_CONVERSION_INJECTED	= ADC_SR_JEOC,	//!< End of conversion of an injected group
				ANALOG_WATCHDOG				= ADC_SR_AWD,	//!< Analog watchdog status bit is set
#if defined(STM32F2XX) || defined(STM32F4XX) 
				OVERRUN						= ADC_SR_OVR	//!< Overrun (if data are lost)
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
			initialize(Prescaler prescaler=PRESCALER_8)
			{
				// Initialize ADC
				RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;	// start ADC Clock
				ADC1->CR2 |= ADC_CR2_ADON;			// switch on ADC
				setPrescaler(prescaler);
			}
			
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
			static inline void
			setPrescaler(const Prescaler prescaler)
			{
#if defined(STM32F2XX) || defined(STM32F4XX)
				ADC->CCR &= ~(0b11 << 17);				// Clear prescaler
				ADC->CCR |= prescaler << 17;		// set prescaler
#elif defined(STM32F10X)
				RCC->CFGR &= ~(0b11 << 14);			// Clear prescaler
				RCC->CFGR |= prescaler << 14;		// set prescaler
#endif
			}
			
#if defined(TEMPERATURE_REFVOLTAGE_AVIALABLE) || defined(__DOXYGEN__)
			/** Switch on temperature- and V_REF measurement. */
			static inline void
			enableTemperatureRefVMeasurement(void)
			{
#if defined(STM32F2XX) || defined(STM32F4XX)
				ADC->CCR |= ADC_CCR_TSVREFE;
#elif defined(STM32F10X)
				ADC1->CR2 |= ADC_CR2_TSVREFE;
#endif
			}

			/** Switch on temperature- and V_REF measurement. */
			static inline void
			disableTemperatureRefVMeasurement(void)
			{
#if defined(STM32F2XX) || defined(STM32F4XX)
				ADC->CCR &= ~ADC_CCR_TSVREFE;
#elif defined(STM32F10X)
				ADC1->CR2 &= ~ADC_CR2_TSVREFE;
#endif
			}
#endif // TEMPERATURE_REFVOLTAGE_AVIALABLE

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
			static inline void
			setLeftAdjustResult(const bool enable)
			{
				if (enable) {
					ADC1->CR2 |= ADC_CR2_ALIGN;
				}
				else {
					ADC1->CR2 &= ~ADC_CR2_ALIGN;
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
			static void
			setChannel(const Channels channel, const SampleTime sampleTime=static_cast<SampleTime>(0b000));

			/**
			 * Enables free running mode
			 *
			 * The ADC will continously start conversions and provide the most
			 * recent result in the ADC register.
			 * 
			 * @pre The ADC clock must be started and the ADC switched on with 
			 * 		initialize()
			 */
			static inline void
			enableFreeRunningMode(void)
			{
				ADC1->CR2 |= ADC_CR2_CONT;	// set to continuous mode
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
			static inline void
			disableFreeRunningMode(void)
			{
				ADC1->CR2 &= ~ADC_CR2_CONT;		// set to single mode
			}

			/**
			 * Returns if the specified interrupt flag is set.
			 * 
			 * @return \c true if the flag is set
			 * @pre The ADC clock must be started and the ADC switched on with 
			 * 	initialize()
			 * @param flag The interrupt flag, which shall be checked.
			 */
			static inline bool
			isInterruptFlagSet(const Interrupt flag)
			{
				return ADC1->SR & flag;
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
			static inline void
			resetInterruptFlags(const Interrupt flag)
			{
				ADC1->SR &= ~flag;
			}

			/**
			 * Disables the ADC Conversion Complete Interrupt.
			 */
			static void
			disableInterrupt(const Interrupt interrupt);

			/**
			 * Enables the ADC Conversion Complete Interrupt.
			 *
			 * You could catch the interrupt using this example function:
			 * \li for STM32F4XX: \code extern "C" void ADC_IRQHandler() \endcode
			 * \li for STM32F10X: \code extern "C" void ADC1_2_IRQHandler() \endcode
			 * 
			 * @pre The ADC clock must be started and the ADC switched on with 
			 * 	initialize()
			 * 
			 * @param priority Priority to set
			 * @param interrupt The interrupt, which shall be enabled. See 
			 * 	Interrupt for available interrupts.
			 * 
			 * @note ADC1 and ADC2 interrupts are mapped onto the same interrupt
			 * 	vector. ADC3 interrupts are mapped onto a separate interrupt 
			 * 	vector.
			 */
			static void
			enableInterrupt(const Interrupt interrupt, const uint32_t priority);
			
			/**
			 * Turns off the ADC and its clock.
			 */
			static inline void
			shutdownAdc(void)
			{
				ADC1->CR2 &= ~(ADC_CR2_ADON);		// switch off ADC
				RCC->APB2ENR &= ~RCC_APB2ENR_ADC1EN; // stop ADC Clock
			}

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
			static inline void
			startConversion(void)
			{
#if defined(STM32F2XX) || defined(STM32F4XX)
				resetInterruptFlags(static_cast<Interrupt>(
						END_OF_CONVERSION_REGULAR | END_OF_CONVERSION_INJECTED |
						ANALOG_WATCHDOG | OVERRUN));
#elif defined(STM32F10X)
				resetInterruptFlags(static_cast<Interrupt>(
						END_OF_CONVERSION_REGULAR | END_OF_CONVERSION_INJECTED |
						ANALOG_WATCHDOG));
				
				// select the SWSTART event used to trigger the start of
				// conversion of a regular group
				ADC1->CR2 |= ADC_CR2_EXTTRIG | ADC_CR2_EXTSEL_0 |
						ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_2;
#endif
				// starts single conversion for the regular group
				ADC1->CR2 |= ADC_CR2_SWSTART;
			}

			/** 
			 * @return If the conversion is finished.
			 * @pre A conversion should have been started with startConversion()
			 */
			static inline bool
			isConversionFinished(void)
			{
				return (ADC1->SR & ADC_SR_EOC);
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
			static inline uint16_t
			getValue(void)
			{
				return ADC1->DR;
			}
		};
	}

}

#endif	// XPCC_STM32__ADC1_HPP
