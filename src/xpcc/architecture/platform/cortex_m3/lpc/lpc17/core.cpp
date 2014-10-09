/*
 * core.cpp
 *
 *  Created on: Mar 31, 2013
 *      Author: walmis
 */

#include <xpcc/architecture.hpp>
#include <xpcc/processing.hpp>
#include <xpcc/container.hpp>
#include <xpcc/debug.hpp>

bool xpcc::lpc17::debugIrq = false;

struct irqCounter {
	int irqn;
	uint32_t count;
	xpcc::Timestamp last;
};

void debug_irqs(int irqn) {
	static xpcc::LinkedList<irqCounter> counts;

	bool found = false;
	irqCounter *cnt = 0;
	for(auto &i : counts) {
		if(i.irqn == irqn) {
			cnt = &i;
			found = true;
		}
	}
	if(!found) {
		irqCounter c;
		c.irqn = irqn;
		c.count = 1;
		c.last = xpcc::Clock::now();

		counts.append(c);
		XPCC_LOG_DEBUG .printf(">IRQ%d\n", irqn);
	} else {
		//XPCC_LOG_DEBUG .printf(">IRQ%d\n", xpcc::Clock::now() - cnt->last);

		if(xpcc::Clock::now() - cnt->last > 100 && cnt->count < 3) {
			XPCC_LOG_DEBUG .printf(">IRQ%d\n", irqn);
			cnt->last = xpcc::Clock::now();
			cnt->count = 0;
		}

		if(xpcc::Clock::now() - cnt->last > 1000){
			if(cnt->count > 2) {
				XPCC_LOG_DEBUG .printf(">IRQ%d (count %d)\n", irqn, cnt->count);
				cnt->count = 0;
				cnt->last = xpcc::Clock::now();
			}
		}
		cnt->count++;

	}
}

bool xpcc::isInterruptContext() {
	return __get_IPSR() != 0;
}

bool xpcc::TickerTask::inInterruptContext() {
	return __get_IPSR() != 0;
}

extern "C" void default_irq_handler() {
	int irqn = __get_IPSR() - 16;

	xpcc::TickerTask::interrupt(irqn);

	if (XPCC_LOG_LEVEL <= xpcc::log::DEBUG) {
		if(xpcc::lpc17::debugIrq) {
			debug_irqs(irqn);
		}
	}
}



