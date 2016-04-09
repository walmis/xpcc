/*
 * semaphore.h
 *
 *  Created on: Oct 12, 2014
 *      Author: walmis
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <stdint.h>
#include <ch.hpp>

namespace xpcc {

class Semaphore {
public:

	void give() {
		syssts_t s = chSysGetStatusAndLockX();
		sem.signalI();
		chSysRestoreStatusX(s);
	}

	bool take(uint32_t timeout_ms = TIME_INFINITE) __attribute__ ((warn_unused_result)) {
		if(timeout_ms == TIME_INFINITE)
			return sem.wait(TIME_INFINITE) == MSG_OK;

		return sem.wait(MS2ST(timeout_ms)) == MSG_OK;
	}
	bool take_nonblocking() __attribute__ ((warn_unused_result)) {
		return sem.wait(TIME_IMMEDIATE) == MSG_OK;
	}

	bool taken() {
		syssts_t s = chSysGetStatusAndLockX();
		bool state = sem.getStateI();
		chSysRestoreStatusX(s);
		return state;
	}

private:
	chibios_rt::BinarySemaphore sem{false};
};

}
#endif /* SEMAPHORE_H_ */
