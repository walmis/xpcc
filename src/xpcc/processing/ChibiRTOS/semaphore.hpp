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
	Semaphore();

	void give();
	bool take(uint16_t timeout = 0) __attribute__ ((warn_unused_result));
	bool take_nonblocking() __attribute__ ((warn_unused_result));

	bool taken();

private:
	chibios_rt::BinarySemaphore sem;
};

}
#endif /* SEMAPHORE_H_ */
