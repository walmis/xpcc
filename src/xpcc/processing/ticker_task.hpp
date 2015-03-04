/*
 * task.h
 *
 *  Created on: Mar 30, 2013
 *      Author: walmis
 */

#ifndef TASK_H_
#define TASK_H_

#include "function.hpp"
#include "timestamp.hpp"
#include "../io/iostream.hpp"

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
	virtual void handleInterrupt(int irqn) {}

	inline void stop() {
		_clearFlag(FLAG_RUNNING);
	}

	inline bool isRunning() {
		return _getFlag(FLAG_RUNNING);
	}

	inline void start() {
		_setFlag(FLAG_RUNNING);
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

	inline bool _getFlag(uint8_t flag) {
		return flags & flag;
	}

	inline void _setFlag(uint8_t flag) {
		flags |= flag;
	}

	inline void _clearFlag(uint8_t flag) {
		flags &= ~flag;
	}

public:
	enum {
		FLAG_RUNNING  = 1,
		FLAG_BLOCKING = 2,
		FLAG_SLEEPING = 4,
		FLAG_YIELDING = 8,
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
	static void tasksRun(xpcc::function<void()> idleFunc = 0);

	static void printTasks(IOStream& stream);
};



}



#endif /* TASK_H_ */
