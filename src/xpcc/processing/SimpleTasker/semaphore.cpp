/*
 * semaphore.cpp
 *
 *  Created on: Oct 12, 2014
 *      Author: walmis
 */

#include "semaphore.hpp"
#include "../ticker_task.hpp"
#include <xpcc/architecture.hpp>

xpcc::Semaphore::Semaphore() : _taken(false) {
}

void xpcc::Semaphore::give() {
	_taken = false;
}

bool xpcc::Semaphore::take(uint16_t timeout_ms) {
    /* Try to take immediately */
    if (take_nonblocking()) {
        return true;
    } else if (timeout_ms == 0) {
        /* Return immediately if timeout is 0 */
        return false;
    }
    xpcc::Timeout<> t(timeout_ms);
    while(!t.isExpired()) {
        if (take_nonblocking()) {
            return true;
        }
        yield();
    }

    return false;
}

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
bool xpcc::Semaphore::take_nonblocking() {

	  volatile unsigned char failed = 1;
	  register bool lock;

	  lock = __LDREXB((uint8_t*)&_taken);
	  if(!lock) {
		  failed = __STREXB(1, (uint8_t*)&_taken);
		  __DMB();
	  }

	  return !failed;
}
#else
bool xpcc::Semaphore::take_nonblocking() {
	xpcc::atomic::Lock l;
	if(_taken) return false;
	_taken = true;
	return true;
}

#endif
