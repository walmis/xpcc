/*
 * debug.hpp
 *
 *  Created on: Mar 4, 2013
 *      Author: walmis
 */

#ifndef DEBUG_HPP_
#define DEBUG_HPP_

#include <xpcc/debug.hpp>
#include <xpcc/processing.hpp>
#include <lpc17xx/cmsis/LPC17xx.h>
#include "rit_clock.hpp"
#define PROFILE() xpcc::Profiler __profiler__(__FUNCTION__)

namespace xpcc {

class ProfileTimer {
public:

	void start() {
		time = lpc17::RitClock::now();
	}

	void end() {
		xpcc::Timestamp etime = lpc17::RitClock::now();
		uint32_t t = (etime - time).getTime()*(1000000000/SystemCoreClock);

		XPCC_LOG_DEBUG .printf("Timer %x took %d.%03d us\n", this, t/1000, t % 1000);
	}

	xpcc::Timestamp time;
};

class Profiler {
public:
	Profiler(const char* name) {
		time = lpc17::RitClock::now();
		n = name;
	}
	~Profiler() {
		xpcc::Timestamp etime = lpc17::RitClock::now();
		uint32_t t = (etime - time).getTime()*(1000000000/SystemCoreClock);

		XPCC_LOG_DEBUG .printf("%s() Took %d.%03d us\n", n, t/1000, t % 1000);
	}
	xpcc::Timestamp time;
	const char* n;
};

}
#endif /* DEBUG_HPP_ */
