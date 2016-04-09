/*
 * event.hpp
 *
 *  Created on: May 28, 2015
 *      Author: walmis
 */

#ifndef SRC_XPCC_PROCESSING_EVENT_HPP_
#define SRC_XPCC_PROCESSING_EVENT_HPP_

#include "semaphore.hpp"

namespace xpcc {

class Event {
public:
	bool wait(uint32_t timeout_ms = 0xFFFFFFFF)  {
		if(TickerTask::inInterruptContext()) return false;

		return sem.take(timeout_ms);
	}
	void signal() {
		sem.give();
	}

	bool isPending() {
		return sem.take_nonblocking();
	}

	void reset() {
		(bool)isPending();
	}
protected:
	Semaphore sem;
};

}


#endif /* SRC_XPCC_PROCESSING_EVENT_HPP_ */
