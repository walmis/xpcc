/*
 * core.cpp
 *
 *  Created on: Mar 31, 2013
 *      Author: walmis
 */

#include <xpcc/architecture.hpp>
#include <xpcc/workflow.hpp>
#include <xpcc/container.hpp>
#include <xpcc/workflow.hpp>
#include <xpcc/debug.hpp>

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
			i.count ++;
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

extern "C" void default_irq_handler() {
	int irqn = __get_IPSR() - 16;
	if (xpcc::log::DEBUG <= xpcc::log::DEBUG) {
		debug_irqs(irqn);
	}
	xpcc::TickerTask::interrupt(irqn);
}



