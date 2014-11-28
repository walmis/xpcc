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

namespace xpcc {

class CoopTask : public xpcc::TickerTask {

public:
	CoopTask(void* stack, size_t stacksize);

	//implemented architecture specific
	static void contextSwitch(void *arg);
protected:
	virtual void run() = 0;
	void _yield(uint16_t timeAvailable);

	void handleTick() override;

	void _thread() {
		while(1) {
			run();
		}
	}
	void* stackPtr;
	void* stack;
	uint16_t stacksize;
	xpcc::Timestamp sleep_timeout;
};

template <typename T, unsigned int STACKSZ>
class CoopWrapper : public CoopTask, public T {
public:
	CoopWrapper() :	CoopTask(_stack, STACKSZ) {}

protected:
	void handleTick() override {
		this->CoopTask::handleTick();
	}

	void handleInit() override {
		XPCC_LOG_DEBUG .printf("init %x %x\n", this, _stack);
		T::handleInit();
	}

	void run() {
		while(1) {
			T::handleTick();
			this->CoopTask::_yield(0);
		}
	}

	void _yield(uint16_t timeAvailable) override {
		this->CoopTask::_yield(timeAvailable);
	}

private:
	uint32_t _stack[STACKSZ/4];
};

}


#endif /* TICKERTASK2_HPP_ */
