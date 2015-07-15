/*
 * semaphore.h
 *
 *  Created on: Oct 12, 2014
 *      Author: walmis
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <stdint.h>
#include "../timeout.hpp"

namespace xpcc {

class Semaphore {
public:
	Semaphore();

	void give();
	bool take(uint16_t timeout = 0xFFFF) __attribute__ ((warn_unused_result));
	bool take_nonblocking() __attribute__ ((warn_unused_result));

	inline bool taken() {
		return _taken;
	}

private:
	volatile bool _taken;
};

}
#endif /* SEMAPHORE_H_ */
