/*
 * task.h
 *
 *  Created on: Mar 30, 2013
 *      Author: walmis
 */

#ifndef TASK_H_
#define TASK_H_

#include "function.hpp"

namespace xpcc {
  
/**
* \brief	Task base for cooperative multitasking
*
* \author	Valmantas Palikša
* \ingroup	workflow
*/ 

void yield(uint16_t timeAvailable = 0);
void sleep(uint16_t time_ms);

class TickerTask {
protected:
	TickerTask(); //constructs the Task and adds itself to tasks List

	virtual ~TickerTask(); //when the task is destructed, it will remove itself from the tasks list and will be no longer processes

	///handle task tick. This is called repeatedly when TickerTask::tasksRun is executing.
	virtual void handleTick() {};

	virtual void handleInit() {};

	///handle interrupt with interrupt code irqn
	virtual void handleInterrupt(int irqn) {
	}

	/// returns true if current task is blocking (yielding)
	inline bool taskBlocking() {
		return flags & FLAG_BLOCKING;
	}
	inline void setBlocking() {
		flags |= FLAG_BLOCKING;
	}
	inline void clearBlocking() {
		flags &= ~FLAG_BLOCKING;
	}

	inline bool taskSleeping() {
		return flags & FLAG_SLEEPING;
	}
	inline void setSleeping() {
		flags |= FLAG_SLEEPING;
	}
	inline void clearSleeping() {
		flags &= ~FLAG_SLEEPING;
	}

	static bool inInterruptContext();

	///internal pointer to the next tickerTask
	TickerTask *next;

	volatile uint8_t flags;

private:
	static TickerTask* base;
	static TickerTask* volatile current;
	static function<void()> idleFunc;


	virtual void _yield(uint16_t timeAvailable);

public:
	enum {
		FLAG_BLOCKING = 1,
		FLAG_SLEEPING = 2,
		FLAG_YIELDING = 4
	};

	static void tick();

	///yield current task. Call this function repeatedly until some blocking operation finishes.
	///note: other tasks are moved down the stack. So watch the stack usage if multiple tasks are blocking.
	static void yield(uint16_t timeAvailable = 0);
	static void sleep(uint16_t time_ms);

	///called from IRQ handler
	static void interrupt(int irqN);

	///Start infinite loop processing all tasks repeatedly. This will cause all tasks' TickerTask::handleTick to be called repeatedly.
	///@param idleFunc function to be executed when all tasks in the chain are executed. Useful to put the processor to sleep if no work is to be done
	static void tasksRun(xpcc::function<void()> idleFunc = 0) {
		TickerTask::idleFunc = idleFunc;
		TickerTask* task = base;
		while (task) {
			task->handleInit();
			task = task->next;
		}

		while(1) {
			tick();
		}
	}
};



}



#endif /* TASK_H_ */
