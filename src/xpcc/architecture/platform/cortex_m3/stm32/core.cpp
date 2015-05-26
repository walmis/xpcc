/*
 * core.cpp
 *
 *  Created on: May 15, 2015
 *      Author: walmis
 */


#include "core.hpp"
#include "device.hpp"
#include <xpcc/processing/ticker_task.hpp>

bool xpcc::TickerTask::inInterruptContext() {
	return __get_IPSR() != 0;
}
