/*
 * core.cpp
 *
 *  Created on: Mar 31, 2013
 *      Author: walmis
 */

#include <xpcc/architecture.hpp>
#include <xpcc/workflow.hpp>

extern "C" void default_irq_handler() {
	xpcc::TickerTask::interrupt(__get_IPSR() - 16);
}

