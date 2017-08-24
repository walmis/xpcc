/*
 * event.hpp
 *
 *  Created on: May 28, 2015
 *      Author: walmis
 */

#ifndef SRC_XPCC_PROCESSING_EVENT_HPP_
#define SRC_XPCC_PROCESSING_EVENT_HPP_

#include "../../ticker_task.hpp"

namespace xpcc {

/*
 * Event handling structure
 *
 * Thread calls Wait to wait for some event occured in interrupt.
 * Eg. character received from UART
 * The IRQ handler then calls event.signal() to notify the listener
 *
 */

class Event  {
public:
	Event();
	~Event();

	bool isPending();
	void reset();
	bool wait(uint32_t timeout_ms = 0xFFFFFFFF);
	bool wait_us(uint32_t timeout_us = 0xFFFFFFFF);

	//signal waiting threads
	void signal();

protected:
	void* event;
};

}


#endif /* SRC_XPCC_PROCESSING_EVENT_HPP_ */
