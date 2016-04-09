/*
 * rtos.hpp
 *
 *  Created on: May 28, 2015
 *      Author: walmis
 */

#ifndef SRC_XPCC_PROCESSING_RTOS_HPP_
#define SRC_XPCC_PROCESSING_RTOS_HPP_

#define XPCC_RTOS SIMPLETASKER

#define SIMPLETASKER 0
#define CHIBI 1

#if XPCC_RTOS_CHIBI
#include <ch.h>
#include <ch.hpp>

#include "RTOSAbstraction/ChibiRTOS/semaphore.hpp"
#include "RTOSAbstraction/ChibiRTOS/event.hpp"
#include "RTOSAbstraction/ChibiRTOS/interrupt_wrapper.hpp"
namespace xpcc {
static ALWAYS_INLINE void yield(uint16_t timeAvailable = 0) {
	chThdYield();
}

static ALWAYS_INLINE void sleep(uint16_t time_ms) {
	chThdSleep(MS2ST(time_ms));
}
}
#else

#include "ticker_task.hpp"
#include "RTOSAbstraction/SimpleTasker/semaphore.hpp"
#include "RTOSAbstraction/SimpleTasker/event.hpp"
#include "RTOSAbstraction/SimpleTasker/interrupt_wrapper.hpp"
namespace xpcc {
static ALWAYS_INLINE void yield(uint16_t timeAvailable = 0) {
	xpcc::TickerTask::yield(timeAvailable);
}

static ALWAYS_INLINE void sleep(uint16_t time_ms) {
	xpcc::TickerTask::sleep(time_ms);
}
}
#endif

#endif /* SRC_XPCC_PROCESSING_RTOS_HPP_ */
