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
	void wait();
	void signal();

protected:
	Semaphore sem;
};

}


#endif /* SRC_XPCC_PROCESSING_EVENT_HPP_ */
