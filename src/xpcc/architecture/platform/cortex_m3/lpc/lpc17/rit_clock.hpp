/*
 * rit_clock.hpp
 *
 *  Created on: Apr 11, 2014
 *      Author: walmis
 */

#ifndef RIT_CLOCK_HPP_
#define RIT_CLOCK_HPP_

#include "../lpc17xx.hpp"

namespace xpcc {
namespace lpc17 {

	class RitClock
	{
	public:

		static void initialize() {
			CLKPwr::setClkPower(CLKPwr::PType::PCRIT, true);
			CLKPwr::setClkDiv(CLKPwr::ClkType::RIT, CLKPwr::ClkDiv::DIV_1);

			LPC_RIT->RICTRL = (1<<3);
		}

		static ALWAYS_INLINE
		Timestamp
		now() {
			return LPC_RIT->RICOUNTER;
		}

	};
}
}


#endif /* RIT_CLOCK_HPP_ */
