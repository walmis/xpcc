/*
 * debug.hpp
 *
 *  Created on: Mar 4, 2013
 *      Author: walmis
 */

#ifndef DEBUG_HPP_
#define DEBUG_HPP_

#include <xpcc/debug.hpp>
#include <lpc17xx/cmsis/LPC17xx.h>

#define PROFILE() xpcc::Profiler __profiler__(__FUNCTION__)

namespace xpcc {
class Profiler {
public:
	Profiler(const char* name) {
		time = xpcc::Clock::now();
		tick = SysTick->VAL;
		n = name;
	}
	~Profiler() {
		uint32_t etick = SysTick->VAL;
		xpcc::Timestamp etime = xpcc::Clock::now();

		int32_t t = (etime.getTime() - time.getTime())*1000000 +
				tick*(1000000000/SystemCoreClock)
				- etick*(1000000000/SystemCoreClock);

		XPCC_LOG_DEBUG .printf("%s() Took %d.%03d us\n", n, t/1000, t % 1000);
	}
	uint32_t tick;
	xpcc::Timestamp time;
	const char* n;
};

}
#endif /* DEBUG_HPP_ */
