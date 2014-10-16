/*
 * task.h
 *
 *  Created on: Mar 30, 2013
 *      Author: walmis
 */

#ifndef TASK_H_
#define TASK_H_

#include <functional>

namespace xpcc {
  
/**
* \brief	Task base for cooperative multitasking
*
* \author	Valmantas Palikša
* \ingroup	workflow
*/ 
__attribute__((weak))
void yield(uint16_t timeAvailable = 0);

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
	inline bool taskBusy() {
		return blocking;
	}

	bool inInterruptContext();

	///internal pointer to the next tickerTask
	TickerTask *next;

private:
	static TickerTask* base;
	static volatile TickerTask* current;
	static std::function<void()> idleFunc;
	volatile bool blocking;

public:
	static void tick();

	///yield current task. Call this function repeatedly until some blocking operation finishes.
	///note: other tasks are moved down the stack. So watch the stack usage if multiple tasks are blocking.
	static void yield(uint16_t timeAvailable = 0);

	///called from IRQ handler
	static void interrupt(int irqN);

	///Start infinite loop processing all tasks repeatedly. This will cause all tasks' TickerTask::handleTick to be called repeatedly.
	///@param idleFunc function to be executed when all tasks in the chain are executed. Useful to put the processor to sleep if no work is to be done
	static void tasksRun(std::function<void()> idleFunc = 0) {
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
