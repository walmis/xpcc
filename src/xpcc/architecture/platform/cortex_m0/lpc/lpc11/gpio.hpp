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

#ifndef XPCC_LPC11XX__GPIO_HPP
#define XPCC_LPC11XX__GPIO_HPP

#include <xpcc/architecture/driver/gpio.hpp>

// All these pins are not GPIOs as default.
// Some special handling is necessary.
// To use them as GPIO the LPC_IOCON register must be set.
#define PIO0_0		RESET_PIO0_0
#define PIO0_10		SWCLK_PIO0_10
#define PIO0_11		R_PIO0_11

#define PIO1_0		R_PIO1_0
#define PIO1_1		R_PIO1_1
#define PIO1_2		R_PIO1_2
#define PIO1_3		SWDIO_PIO1_3

enum Port {
	PIO0,
	PIO1,
	PIO2,
	PIO3
};

/*
 * 12-bit ports.
 */
namespace xpcc
{
	namespace lpc
	{
		/**
		 * \ingroup	lpc11xx
		 */
		enum InputType
		{
			FLOATING = (0 << 3),
			PULLDOWN = (1 << 3),
			PULLUP   = (2 << 3),
			REPEATER = (3 << 3),
		};
		
		/**
		 * \ingroup	lpc11xx
		 */
		enum OutputType
		{
			PUSH_PULL  = (0 << 10),
			OPEN_DRAIN = (1 << 10),
		};
	}
}

#define CASE_PIN(port, pin) case pin: return &LPC_IOCON->CONCAT4(PIO, port, _, pin)

inline
volatile uint32_t* _get_pin(uint8_t port, uint8_t pin) {
	switch(port) {
	case 0: switch(pin) {
			CASE_PIN(0, 0);
			CASE_PIN(0, 1);
			CASE_PIN(0, 2);
			CASE_PIN(0, 3);
			CASE_PIN(0, 4);
			CASE_PIN(0, 5);
			CASE_PIN(0, 6);
			CASE_PIN(0, 7);
			CASE_PIN(0, 8);
			CASE_PIN(0, 9);
			CASE_PIN(0, 10);
			CASE_PIN(0, 11);
		}

		case 1: switch(pin) {
			CASE_PIN(1, 0);
			CASE_PIN(1, 1);
			CASE_PIN(1, 2);
			CASE_PIN(1, 3);
			CASE_PIN(1, 4);
			CASE_PIN(1, 5);
			CASE_PIN(1, 6);
			CASE_PIN(1, 7);
			CASE_PIN(1, 8);
			CASE_PIN(1, 9);
			CASE_PIN(1, 10);
			CASE_PIN(1, 11);
		};

		case 2: switch(pin) {
			CASE_PIN(2, 0);
			CASE_PIN(2, 1);
			CASE_PIN(2, 2);
			CASE_PIN(2, 3);
			CASE_PIN(2, 4);
			CASE_PIN(2, 5);
			CASE_PIN(2, 6);
			CASE_PIN(2, 7);
			CASE_PIN(2, 8);
			CASE_PIN(2, 9);
			CASE_PIN(2, 10);
			CASE_PIN(2, 11);
		}

		case 3: switch(pin) {
			CASE_PIN(3, 0);
			CASE_PIN(3, 1);
			CASE_PIN(3, 2);
			CASE_PIN(3, 3);
			CASE_PIN(3, 4);
			CASE_PIN(3, 5);
		}
	}

	while(1);
}

template <int Port, int Pin>
class GPIO {
public:
		static const int port = Port;
		static const int pin = Pin;

		inline static void
		setOutput(bool status) {
			setOutput();
			set(status);
		}

		inline static void
		setOutput(::xpcc::lpc::OutputType type = ::xpcc::lpc::PUSH_PULL) {
			*_get_pin(Port, Pin) &= ~0b1111;
			if(Port == 1) {
				if(Pin == 0 || Pin == 1 || Pin == 2 || Pin == 3) {
					uint32_t temp = *_get_pin(Port, Pin);
					temp &= ~0x7;
					temp |= 1;
					*_get_pin(Port, Pin) = temp;
				}
			}
			if(Port == 0) {
				if(Pin == 0 || Pin == 10 || Pin == 11) {
					uint32_t temp = *_get_pin(Port, Pin);
					temp &= ~0x7;
					temp |= 1;
					*_get_pin(Port, Pin) = temp;
				}
			}
			*_get_pin(Port, Pin) |= type;

			switch(Port) {
				case 0: LPC_GPIO0->DIR |= 1 << Pin; break;
				case 1: LPC_GPIO1->DIR |= 1 << Pin; break;
				case 2: LPC_GPIO2->DIR |= 1 << Pin; break;
				case 3: LPC_GPIO3->DIR |= 1 << Pin; break;
			}

		}

		inline static void
		setAltFunc(uint8_t altFunc) {
			xpcc::lpc11::IOCon::setPinFunc(Port, Pin, altFunc);
		}

		inline static void
		setMode(PinMode mode) {
			xpcc::lpc11::IOCon::setPinMode(Port, Pin, mode);
		}

		inline static void
		setInput(::xpcc::lpc::InputType type = ::xpcc::lpc::PULLUP) {
			*_get_pin(Port, Pin) &= ~0b1111;
			if(Port == 0) {
				if(Pin == 0 || Pin == 10 || Pin == 11) {
					uint32_t temp = *_get_pin(Port, Pin);
					temp &= ~0x7;
					temp |= 1;
					*_get_pin(Port, Pin) = temp;
				}
			} else
			if(Port == 1) {
				if(Pin == 0 || Pin == 1 || Pin == 2 || Pin == 3) {
					uint32_t temp = *_get_pin(Port, Pin);
					temp &= ~0x7;
					temp |= 1;
					*_get_pin(Port, Pin) = temp;
				}
			}

			*_get_pin(Port, Pin) |= type;
			switch(Port) {
				case 0: LPC_GPIO0->DIR &= ~(1 << Pin); break;
				case 1: LPC_GPIO1->DIR &= ~(1 << Pin); break;
				case 2: LPC_GPIO2->DIR &= ~(1 << Pin); break;
				case 3: LPC_GPIO3->DIR &= ~(1 << Pin); break;
			}
		}

		inline static void set()    {
			switch(Port) {
				case 0: LPC_GPIO0->MASKED_ACCESS[1 << Pin] = 1<<Pin; break;
				case 1: LPC_GPIO1->MASKED_ACCESS[1 << Pin] = 1<<Pin; break;
				case 2: LPC_GPIO2->MASKED_ACCESS[1 << Pin] = 1<<Pin; break;
				case 3: LPC_GPIO3->MASKED_ACCESS[1 << Pin] = 1<<Pin; break;
			}
		}

		inline static void reset()  {
			switch(Port) {
				case 0: LPC_GPIO0->MASKED_ACCESS[1 << Pin] = 0; break;
				case 1: LPC_GPIO1->MASKED_ACCESS[1 << Pin] = 0; break;
				case 2: LPC_GPIO2->MASKED_ACCESS[1 << Pin] = 0; break;
				case 3: LPC_GPIO3->MASKED_ACCESS[1 << Pin] = 0; break;
			}
		}

		inline static void toggle()         { if (read()) { reset(); } else {   set(); } }
		inline static void set(bool status) { if (status) {   set(); } else { reset(); } }

		inline static bool read()     {
			switch(Port) {
				case 0: return LPC_GPIO0->MASKED_ACCESS[1 << Pin] >> Pin; break;
				case 1: return LPC_GPIO1->MASKED_ACCESS[1 << Pin] >> Pin; break;
				case 2: return LPC_GPIO2->MASKED_ACCESS[1 << Pin] >> Pin; break;
				case 3: return LPC_GPIO3->MASKED_ACCESS[1 << Pin] >> Pin; break;
			}
			return 0;
		}
};

/**
 * \ingroup	lpc11xx
 * \brief	Create a input/output pin type
 * 
 * \hideinitializer
 */
#define	GPIO__IO(name, port, pin) \
	struct name { \
		static const int Port = port; \
		static const int Pin = pin; \
		ALWAYS_INLINE static void \
		setOutput(bool status) { \
			setOutput(); \
			set(status); \
		} \
		ALWAYS_INLINE static void \
		setOutput(::xpcc::lpc::OutputType type = ::xpcc::lpc::PUSH_PULL) { \
			LPC_IOCON->CONCAT4(PIO, port, _, pin) &= ~0b1111;\
			if(port == 1) { \
				if(pin == 0 || pin == 1 || pin == 2 || pin == 3) { \
					uint32_t temp = LPC_IOCON->CONCAT4(PIO, port, _, pin); \
					temp &= ~0x7; \
					temp |= 1; \
					LPC_IOCON->CONCAT4(PIO, port, _, pin) = temp; \
				}\
			}\
			if(port == 0) { \
				if(pin == 0 || pin == 10 || pin == 11) { \
					uint32_t temp = LPC_IOCON->CONCAT4(PIO, port, _, pin); \
					temp &= ~0x7; \
					temp |= 1; \
					LPC_IOCON->CONCAT4(PIO, port, _, pin) = temp; \
				}\
			}\
			LPC_IOCON->CONCAT4(PIO, port, _, pin) |= type; \
			CONCAT(LPC_GPIO, port)->DIR |= 1 << pin; \
		} \
		ALWAYS_INLINE static void \
		setInput(::xpcc::lpc::InputType type = ::xpcc::lpc::PULLUP) { \
			LPC_IOCON->CONCAT4(PIO, port, _, pin) &= ~0b1111;\
			if(port == 1) { \
				if(pin == 0 || pin == 1 || pin == 2 || pin == 3) { \
					uint32_t temp = LPC_IOCON->CONCAT4(PIO, port, _, pin); \
					temp &= ~0x7; \
					temp |= 1; \
					LPC_IOCON->CONCAT4(PIO, port, _, pin) = temp; \
				}\
			}\
			if(port == 0) { \
				if(pin == 0 || pin == 10 || pin == 11) { \
					uint32_t temp = LPC_IOCON->CONCAT4(PIO, port, _, pin); \
					temp &= ~0x7; \
					temp |= 1; \
					LPC_IOCON->CONCAT4(PIO, port, _, pin) = temp; \
				}\
			}\
			LPC_IOCON->CONCAT4(PIO, port, _, pin) |= type; \
			CONCAT(LPC_GPIO, port)->DIR           &= ~(1 << pin); \
		} \
		ALWAYS_INLINE static void set()    { CONCAT(LPC_GPIO, port)->MASKED_ACCESS[1 << pin] = (1 << pin); } \
		ALWAYS_INLINE static void reset()  { CONCAT(LPC_GPIO, port)->MASKED_ACCESS[1 << pin] = 0;          } \
		ALWAYS_INLINE static void toggle()         { if (read()) { reset(); } else {   set(); } } \
		ALWAYS_INLINE static void set(bool status) { if (status) {   set(); } else { reset(); } } \
		ALWAYS_INLINE static bool read()           { return (CONCAT(LPC_GPIO, port)->MASKED_ACCESS[1 << pin]) >> pin; } \
	}

/**
 * \brief	Create a output pin type
 * 
 * Examples:
 * \code
 * GPIO__OUTPUT(Led, 0, 7);
 * 
 * Led::setOutput();
 * Led::setOutput(xpcc::lpc::PUSH_PULL);
 * Led::setOutput(xpcc::lpc::OPEN_DRAIN);
 * 
 * Led::set();
 * Led::reset();
 * 
 * Led::toggle();
 * \endcode
 * 
 * \hideinitializer
 * \ingroup	lpc11xx
 */
#define	GPIO__OUTPUT(name, port, pin) \
	struct name { \
		static const int Port = port; \
		static const int Pin = pin; \
		ALWAYS_INLINE static void \
		setOutput(bool status) { \
			setOutput(); \
			set(status); \
		} \
		ALWAYS_INLINE static void \
		setOutput(::xpcc::lpc::OutputType type = ::xpcc::lpc::PUSH_PULL) { \
			LPC_IOCON->CONCAT4(PIO, port, _, pin) &= ~0b1111;\
			if(port == 1) { \
				if(pin == 0 || pin == 1 || pin == 2 || pin == 3) { \
					LPC_IOCON->CONCAT4(PIO, port, _, pin) |= 1;\
				}\
			}\
			if(port == 0) { \
				if(pin == 0 || pin == 10 || pin == 11) { \
					LPC_IOCON->CONCAT4(PIO, port, _, pin) |= 1;\
				}\
			}\
			LPC_IOCON->CONCAT4(PIO, port, _, pin) |= type; \
			CONCAT(LPC_GPIO, port)->DIR |= 1 << pin; \
		} \
		ALWAYS_INLINE static void set()            { CONCAT(LPC_GPIO, port)->MASKED_ACCESS[1 << pin] = (1 << pin); } \
		ALWAYS_INLINE static void reset()          { CONCAT(LPC_GPIO, port)->MASKED_ACCESS[1 << pin] = 0;          } \
		ALWAYS_INLINE static void toggle()         { if (read()) { reset(); } else {   set(); } } \
		ALWAYS_INLINE static void set(bool status) { if (status) {   set(); } else { reset(); } } \
	protected: \
		ALWAYS_INLINE static bool \
		read() { \
			return (CONCAT(LPC_GPIO, port)->MASKED_ACCESS[1 << pin]) >> pin; \
		} \
	}

/**
 * \brief	Create a input type
 * 
 * Examples:
 * \code
 * GPIO__INPUT(Button, 0, 3);
 * 
 * Button::setInput();
 * Button::setInput(xpcc::lpc::PULLUP);
 * Button::setInput(xpcc::lpc::PULLDOWN);
 * 
 * if (Button::read()) {
 *     ...
 * }
 * \endcode
 * 
 * \hideinitializer
 * \ingroup	lpc11xx
 */
#define GPIO__INPUT(name, port, pin) \
	struct name { \
		static const int Port = port; \
		static const int Pin = pin; \
		ALWAYS_INLINE static void \
		setInput(::xpcc::lpc::InputType type = ::xpcc::lpc::PULLUP) { \
			LPC_IOCON->CONCAT4(PIO, port, _, pin) &= ~0b1111;\
			if(port == 1) { \
				if(pin == 0 || pin == 1 || pin == 2 || pin == 3) { \
					LPC_IOCON->CONCAT4(PIO, port, _, pin) |= 1;\
				}\
			}\
			if(port == 0) { \
				if(pin == 0 || pin == 10 || pin == 11) { \
					LPC_IOCON->CONCAT4(PIO, port, _, pin) |= 1;\
				}\
			}\
			LPC_IOCON->CONCAT4(PIO, port, _, pin) |= type; \
			CONCAT(LPC_GPIO, port)->DIR           &= ~(1 << pin); \
		} \
		ALWAYS_INLINE static bool \
		read() { \
			return (CONCAT(LPC_GPIO, port)->MASKED_ACCESS[1 << pin]) >> pin; \
		} \
	}


namespace xpcc
{
	/**
	 * \brief	General purpose input and/or output pins
	 * \ingroup	gpio
	 */
	namespace lpc
	{
	template <typename Pin>
	class Invert
	{
	public:
		ALWAYS_INLINE static void
		setOutput()
		{
			Pin::setOutput();
		}

		ALWAYS_INLINE static void
		setOutput(bool value)
		{
			Pin::setOutput(!value);
		}

		ALWAYS_INLINE static void
		setInput()
		{
			Pin::setInput();
		}

		ALWAYS_INLINE static void
		setInput(xpcc::lpc::InputType t)
		{
			Pin::setInput(t);
		}

		ALWAYS_INLINE static void
		set()
		{
			Pin::reset();
		}

		ALWAYS_INLINE static void
		set(bool value)
		{
			Pin::set(!value);
		}

		ALWAYS_INLINE static void
		reset()
		{
			Pin::set();
		}

		ALWAYS_INLINE static void
		toggle()
		{
			Pin::toggle();
		}

		ALWAYS_INLINE static bool
		read()
		{
			return !Pin::read();
		}
	};

	} // gpio namespace
} // xpcc namespace

#endif // XPCC_LPC11XX__GPIO_HPP
