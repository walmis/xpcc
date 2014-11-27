/*
 * tickertask2.hpp
 *
 *  Created on: Apr 2, 2013
 *      Author: walmis
 */

#ifndef TICKERTASK2_HPP_
#define TICKERTASK2_HPP_

#include <xpcc/processing.hpp>
#include <xpcc/debug.hpp>

class CoopTask : xpcc::TickerTask {

public:
	CoopTask(uint8_t* stack, size_t stacksize) : stack(stack),
			stacksize(stacksize), stackPtr(0) {}


	virtual void run() = 0;


	void _yield(uint16_t timeAvailable) override {

	}

	//implemented architecture specific
	static void contextSwitch(void *arg);
private:
	void handleTick() override;

	void _thread() {
		while(1) {
			run();
		}
	}

	void* stackPtr;
	void* stack;
	uint16_t stacksize;

	volatile uint8_t flags;

	static CoopTask* currTask;
};



#endif /* TICKERTASK2_HPP_ */
