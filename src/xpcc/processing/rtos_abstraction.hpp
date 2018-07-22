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

#include "RTOSAbstraction/ChibiRTOS/semaphore.hpp"
#include "RTOSAbstraction/ChibiRTOS/r_mutex.hpp"
#include "RTOSAbstraction/ChibiRTOS/event.hpp"
#include "RTOSAbstraction/ChibiRTOS/interrupt_wrapper.hpp"
namespace xpcc {
void yield(uint16_t timeAvailable = 0);
void sleep(uint16_t time_ms);
}
#else

#include "RTOSAbstraction/SimpleTasker/semaphore.hpp"
#include "RTOSAbstraction/SimpleTasker/event.hpp"
#include "RTOSAbstraction/SimpleTasker/interrupt_wrapper.hpp"
extern "C" void _delay_ms(uint32_t);
namespace xpcc {
static ALWAYS_INLINE void yield(uint16_t timeAvailable = 0) {
}

static ALWAYS_INLINE void sleep(uint16_t time_ms) {
    _delay_ms(time_ms);
}
}
#endif

#endif /* SRC_XPCC_PROCESSING_RTOS_HPP_ */
