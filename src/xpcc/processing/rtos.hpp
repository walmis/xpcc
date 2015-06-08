/*
 * rtos.hpp
 *
 *  Created on: May 28, 2015
 *      Author: walmis
 */

#ifndef SRC_XPCC_PROCESSING_RTOS_HPP_
#define SRC_XPCC_PROCESSING_RTOS_HPP_

#if 0

#include "SimpleTasker/semaphore.hpp"
#include "SimpleTasker/event.hpp"
#include "SimpleTasker/interrupt_wrapper.hpp"
namespace xpcc {
static ALWAYS_INLINE void yield(uint16_t timeAvailable = 0) {
	xpcc::TickerTask::yield(timeAvailable);
}

static ALWAYS_INLINE void sleep(uint16_t time_ms) {
	xpcc::TickerTask::sleep(time_ms);
}
}
#else
#include <ch.h>
#include <ch.hpp>

#include "ChibiRTOS/semaphore.hpp"
#include "ChibiRTOS/event.hpp"
#include "ChibiRTOS/interrupt_wrapper.hpp"
namespace xpcc {
static ALWAYS_INLINE void yield(uint16_t timeAvailable = 0) {
	xpcc::TickerTask::yield(timeAvailable);
}

static ALWAYS_INLINE void sleep(uint16_t time_ms) {
	chThdSleep(MS2ST(time_ms));
}
}
#endif

#endif /* SRC_XPCC_PROCESSING_RTOS_HPP_ */
