/*
 * event.hpp
 *
 *  Created on: May 28, 2015
 *      Author: walmis
 */

#ifndef SRC_XPCC_PROCESSING_EVENT_HPP_
#define SRC_XPCC_PROCESSING_EVENT_HPP_

#include <ch.hpp>
#include "../ticker_task.hpp"

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
	bool isPending() {
		return cond.wait(TIME_IMMEDIATE) == MSG_TIMEOUT;
	}

	void reset() {
		(bool)isPending();
	}

	bool wait(uint32_t timeout_ms = 0) {
		if(TickerTask::inInterruptContext()) return false;

		//(bool)isPending(); //this resets the event into waiting state

		if(!timeout_ms) {
			return cond.wait() == MSG_OK;
		} else {
			return cond.wait(MS2ST(timeout_ms)) == MSG_OK;
		}
	}

	bool wait_us(uint32_t timeout_us = 0) {
		if(TickerTask::inInterruptContext()) return false;

		//(bool)isPending(); //this resets the event into waiting state

		if(!timeout_us) {
			return cond.wait() == MSG_OK;
		} else {
			return cond.wait(timeout_us) == MSG_OK;
		}
	}

	//signal waiting threads
	void signal() {
		syssts_t s = chSysGetStatusAndLockX();
		cond.signalI();
		chSysRestoreStatusX(s);
	}

protected:
	chibios_rt::BinarySemaphore cond{true};
};

}


#endif /* SRC_XPCC_PROCESSING_EVENT_HPP_ */
