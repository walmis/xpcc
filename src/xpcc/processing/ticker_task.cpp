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
	blocking = false;
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
	if(!current) return;
	TickerTask* t = (TickerTask*)current;
	if(t->inInterruptContext()) return;
	xpcc::Timeout<> tm(timeAvailable);
	//XPCC_LOG_DEBUG .printf("current %x\n", t);
	if(t) {
		t->blocking = true;
		do {
			tick();
		} while(!tm.isExpired());
		t->blocking = false;
	}
	current = t;
}

void TickerTask::tick() {
	static TickerTask* task = 0;
	if(!task) {
		if(idleFunc) idleFunc();
		task = base;
	}
	TickerTask* tsk = task;

	if(!tsk->blocking) {
		current = tsk;
		tsk->handleTick();
		tsk->blocking = false;
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
std::function<void()> TickerTask::idleFunc;
volatile TickerTask* TickerTask::current = 0;
}
