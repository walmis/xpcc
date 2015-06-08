/*
 * semaphore.cpp
 *
 *  Created on: Oct 12, 2014
 *      Author: walmis
 */

#include "semaphore.hpp"
#include "../ticker_task.hpp"
#include <xpcc/architecture.hpp>

xpcc::Semaphore::Semaphore() : sem(false) {
}

void xpcc::Semaphore::give() {
	syssts_t s = chSysGetStatusAndLockX();
	sem.signalI();
	chSysRestoreStatusX(s);
}

bool xpcc::Semaphore::take(uint16_t timeout_ms) {
	return sem.wait(MS2ST(timeout_ms)) == MSG_OK;
}

bool xpcc::Semaphore::taken() {
		syssts_t s = chSysGetStatusAndLockX();
		bool state = sem.getStateI();
		chSysRestoreStatusX(s);
		return s;
}

bool xpcc::Semaphore::take_nonblocking() {
	return sem.wait(TIME_IMMEDIATE) == MSG_OK;
}

