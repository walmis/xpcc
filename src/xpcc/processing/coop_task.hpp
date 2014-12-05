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

class CoopTask_Base {

public:
	CoopTask_Base(void* stack, size_t stacksize);

	//implemented architecture specific
	static void* contextSwitch(void *arg);
protected:
	virtual void run() = 0;
	void _yield(uint16_t timeAvailable);

	void _handleTick();

	void _thread() {
		while(1) {
			run();
		}
	}

	void* sp;
	void* stack;
	uint16_t stacksize;
	xpcc::Timestamp sleep_timeout;
	uint8_t flags;
};

template <typename Task, size_t stack_size>
class CoopTask : public Task, CoopTask_Base {
public:
	template<typename ... Args>
	CoopTask(Args ... args) :
			Task(args...), CoopTask_Base(_stack, stack_size) {
	}

protected:
	void handleTick() override {
		CoopTask_Base::_handleTick();
	}

	void _yield(uint16_t timeAvailable) override {
		CoopTask_Base::_yield(timeAvailable);
	}

	void run() {
		Task::handleTick();
		_yield(0);
	}

	void* _stack[stack_size / sizeof(void*)];
};


}


#endif /* TICKERTASK2_HPP_ */
