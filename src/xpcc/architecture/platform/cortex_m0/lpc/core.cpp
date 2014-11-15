/*
 * core.cpp
 *
 *  Created on: Mar 31, 2013
 *      Author: walmis
 */

#include <xpcc/architecture.hpp>
#include <xpcc/processing.hpp>
#include <xpcc/container.hpp>

namespace xpcc {

	void yield(uint16_t timeAvailable) {
		xpcc::TickerTask::yield(timeAvailable);
	}

	void sleep(uint16_t time_ms) {
		TickerTask::sleep(time_ms);
	}
}

extern "C" void default_irq_handler() {
	int irqn = __get_IPSR() - 16;

	xpcc::TickerTask::interrupt(irqn);
}

