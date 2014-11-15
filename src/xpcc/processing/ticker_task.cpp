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
	clearBlocking();
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
	if(current)
		current->_yield(timeAvailable);
}

void TickerTask::sleep(uint16_t time_ms) {
	Timeout<> t(time_ms);
	while(!t.isExpired()) {
		yield();
	}
}

void TickerTask::_yield(uint16_t timeAvailable) {
	if(inInterruptContext()) return;

	TickerTask* t = current;
	xpcc::Timeout<> tm(timeAvailable);
	//XPCC_LOG_DEBUG .printf("current %x\n", t);
	if(t) {
		t->setBlocking();
		do {
			tick();
		} while(!tm.isExpired());
		t->clearBlocking();
	}
	current = t;
}

void TickerTask::tick() {
	static TickerTask* volatile task = 0;
	if(!task) {
		if(idleFunc) idleFunc();
		task = base;
	}
	TickerTask* tsk = task;

	if(!tsk->taskBlocking()) {
		current = tsk;
		tsk->handleTick();
		//tsk->clearBlocking();
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

TickerTask* TickerTask::base = 0;
xpcc::function<void()> TickerTask::idleFunc;
TickerTask* volatile TickerTask::current = 0;
}
