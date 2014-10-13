/*
 * semaphore.cpp
 *
 *  Created on: Oct 12, 2014
 *      Author: walmis
 */

#include "semaphore.hpp"

xpcc::Semaphore::Semaphore() : _taken(false) {
}

bool xpcc::Semaphore::acquire_blocking(uint16_t timeout) {
	xpcc:Timeout<> t(timeout);
	while(!t.isExpired()) {
		if(acquire()) {
			return true;
		}
	}
	return false;
}
