/*
 * semaphore.h
 *
 *  Created on: Oct 12, 2014
 *      Author: walmis
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <stdint.h>

namespace xpcc {

class Semaphore {
public:
	Semaphore();
	~Semaphore();
	
	void give();

	bool take(uint32_t timeout_ms = 0) __attribute__ ((warn_unused_result));
	bool take_nonblocking() __attribute__ ((warn_unused_result));
	bool taken();

private:
	void* semaphore;
};

}
#endif /* SEMAPHORE_H_ */
