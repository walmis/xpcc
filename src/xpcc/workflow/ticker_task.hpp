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

class TickerTask {
protected:
	TickerTask();

	virtual ~TickerTask();

	//system tick handler
	virtual void handleTick() {};

	//interrupt handler
	virtual void handleInterrupt(int irqn) {
	}

	TickerTask *next;

private:
	static TickerTask* base;

public:
	static void tick();

	//called from IRQ handler
	static void interrupt(int irqN);

	static void tasksRun(std::function<void()> idleFunc = 0) {
		while(1) {
			tick();
			if(idleFunc)
				idleFunc();
		}
	}
};



}



#endif /* TASK_H_ */
