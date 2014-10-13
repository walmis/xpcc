/*
 * semaphore.h
 *
 *  Created on: Oct 12, 2014
 *      Author: walmis
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <stdint.h>
#include "timeout.hpp"

namespace xpcc {

class Semaphore {
	Semaphore();

	void release();
	bool acquire();
	bool acquire_blocking(uint16_t timeout);

private:
	volatile bool _taken;
};

}
#endif /* SEMAPHORE_H_ */
