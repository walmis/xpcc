/*
 * task.h
 *
 *  Created on: Mar 30, 2013
 *      Author: walmis
 */

#ifndef TASK_H_
#define TASK_H_

#include <xpcc/architecture/driver/atomic.hpp>

namespace xpcc {

class TickerTask {
protected:
	TickerTask() {
		xpcc::atomic::Lock lock;
		next = 0;
		if(base == 0) {
			base = this;
		} else {
			TickerTask* t = base;
			while(t) {
				if(t->next == 0) {
					t->next = this;
					break;
				}
				t = t->next;
			}
		}
	}

	virtual ~TickerTask() {
		xpcc::atomic::Lock lock;
		if(base == this) {
			base = next;
		} else {
			TickerTask* t = base;
			while(t) {
				if(t->next == this) {
					t->next = next;
					break;
				}
				t = t->next;
			}
		}
	}

	virtual void run() = 0;

	TickerTask *next;
	static TickerTask* base;

public:
	static void tick() {
		TickerTask* task = base;
		while(task) {
			task->run();
			task = task->next;
		}
	}
};

TickerTask* TickerTask::base = 0;

}



#endif /* TASK_H_ */
