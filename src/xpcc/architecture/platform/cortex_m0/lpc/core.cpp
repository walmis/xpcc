/*
 * core.cpp
 *
 *  Created on: Mar 31, 2013
 *      Author: walmis
 */

#include <xpcc/architecture.hpp>
#include <xpcc/processing.hpp>
#include <xpcc/container.hpp>


bool xpcc::TickerTask::inInterruptContext() {
	return __get_IPSR() != 0;
}

extern "C" void default_irq_handler() {
	int irqn = __get_IPSR() - 16;

	xpcc::TickerTask::interrupt(irqn);
}

