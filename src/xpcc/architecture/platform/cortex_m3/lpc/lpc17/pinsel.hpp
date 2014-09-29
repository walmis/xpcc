/*
 * pinsel.hpp
 *
 *  Created on: Oct 9, 2013
 *      Author: walmis
 */

#ifndef PINSEL_HPP_
#define PINSEL_HPP_

#include <xpcc/architecture.hpp>
namespace xpcc {
namespace lpc17 {

class Pinsel {
public:

	template <typename Pin>
	static void setFunc(int func) {
		setFunc(Pin::Port, Pin::Pin, func);
	}

	static void setFunc(uint8_t port, uint8_t pin, int func) {
		uint32_t pinnum_t = pin;
		uint32_t pinselreg_idx = 2 * port;

		uint32_t *pPinCon = (uint32_t *)&LPC_PINCON->PINSEL0;

		if (pinnum_t >= 16) {
			pinnum_t -= 16;
			pinselreg_idx++;
		}
		*(uint32_t *)(pPinCon + pinselreg_idx) &= ~(0x03UL << (pinnum_t * 2));
		*(uint32_t *)(pPinCon + pinselreg_idx) |= ((uint32_t)func) << (pinnum_t * 2);
	}


	enum ResMode {
		PULLUP = 0,
		TRISTATE = 2,
		PULLDOWN = 3
	};

	template <typename Pin>
	static void setResistorMode (ResMode modenum) {
		setResistorMode(Pin::Port, Pin::Pin, modenum);
	}

	static void setResistorMode (uint8_t port, uint8_t pin, ResMode modenum)
	{
		uint32_t pinnum_t = pin;
		uint32_t pinmodereg_idx = 2 * port;
		uint32_t *pPinCon = (uint32_t *)&LPC_PINCON->PINMODE0;

		if (pinnum_t >= 16) {
			pinnum_t -= 16;
			pinmodereg_idx++ ;
		}

		*(uint32_t *)(pPinCon + pinmodereg_idx) &= ~(0x03UL << (pinnum_t * 2));
		*(uint32_t *)(pPinCon + pinmodereg_idx) |= ((uint32_t)modenum) << (pinnum_t * 2);
	}


};

}
}
#endif /* PINSEL_HPP_ */
