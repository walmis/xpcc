/*
 * ticker_task.cpp
 *
 *  Created on: Mar 31, 2013
 *      Author: walmis
 */

#include "ticker_task.hpp"
#include "timeout.hpp"
#include <xpcc/architecture/driver/atomic.hpp>
#include <xpcc/debug.hpp>
namespace xpcc {

TickerTask::TickerTask() {
	xpcc::atomic::Lock lock;
	flags = 0;
	next = 0;
	if (base == 0) {
		base = this;
	} else {
		TickerTask* t = base;
		while (t) {
			if (t->next == 0) {
				t->next = this;
				break;
			}
			t = t->next;
		}
	}
	_setFlag(FLAG_RUNNING);
}

TickerTask::~TickerTask() {
	xpcc::atomic::Lock lock;
	if (base == this) {
		base = next;
	} else {
		TickerTask* t = base;
		while (t) {
			if (t->next == this) {
				t->next = next;
				break;
			}
			t = t->next;
		}
	}
}

void TickerTask::yield(uint16_t timeAvailable) {
#ifdef TASK_DEBUG
	uint32_t lr;
	asm volatile("mov %[v], lr" : [v]"=r"(lr));
#endif
	if(current) {
#ifdef TASK_DEBUG
		current->yield_return_address = lr;
#endif
		current->_yield(timeAvailable);
	}
}

void TickerTask::sleep(uint16_t time_ms) {
	Timeout<> t(time_ms);
	while(!t.isExpired()) {
		yield(time_ms);
	}
}

void TickerTask::_yield(uint16_t timeAvailable) {
	if(inInterruptContext()) return;

	TickerTask* t = current;
	xpcc::Timeout<> tm(timeAvailable);
	//XPCC_LOG_DEBUG .printf("yield %d\n", timeAvailable);
	if(t) {
		t->_setFlag(FLAG_BLOCKING);
		do {
			tick();
		} while(!tm.isExpired());
		t->_clearFlag(FLAG_BLOCKING);
	}
	current = t;
}

void TickerTask::tick() {
	static TickerTask* volatile task = 0;
	if(!task) {
		current = 0;
		if(idleFunc) idleFunc();
		task = base;
		if(!task) return;
	}
	TickerTask* tsk = task;

	if(tsk->_getFlag(FLAG_RUNNING) &&
			!tsk->_getFlag(FLAG_BLOCKING)) {
		current = tsk;
		tsk->handleTick();
	}
	if(task)
		task = task->next;
}

void TickerTask::interrupt(int irqN) {
	TickerTask* task = base;
	while (task) {
		task->handleInterrupt(irqN);
		task = task->next;
	}
}

void TickerTask::tasksRun(xpcc::function<void()> idleFunc) {
	tasksInit();
	while(1) {
		tick();
	}
}

void TickerTask::tasksInit() {
	TickerTask::idleFunc = idleFunc;
	TickerTask* task = base;
	while (task) {
		task->handleInit();
		task = task->next;
	}
}
#ifdef TASK_DEBUG
void TickerTask::printTasks(IOStream& stream) {
	TickerTask* task = TickerTask::base;
	stream.printf("--- TASKS ---\n");
	while(task) {
		stream.printf("Task(@0x%x) flags:%x ", task, task->flags);

		if(task->_getFlag(FLAG_BLOCKING)) {
			stream.printf("lr:%x", task->yield_return_address);
		}

		stream << endl;
		task = task->next;
	}
	stream.printf("-------------\n");
}
#endif
TickerTask* TickerTask::base = 0;
xpcc::function<void()> TickerTask::idleFunc;
TickerTask* volatile TickerTask::current = 0;
}
