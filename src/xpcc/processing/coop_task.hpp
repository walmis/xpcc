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

	enum {
		TSK_YIELD = 1
	};

	CoopTask(uint16_t stackSize);

	virtual ~CoopTask() {
		if(stack)
			delete (uint8_t*)stack;
	}

	virtual void run() = 0;

	void handleTick() override;

	//implemented architecture specific
	static void contextSwitch(void *arg);
	//implemented architecture specific
	static void yield();

	static void sleep(uint16_t time) {
		xpcc::Timeout<> t(time);
		while(!t.isExpired()) {
			yield();
		}
	}

	class Mutex {
	public:
		Mutex() : flag(0) {}

		void lock() {
			while(flag) {
				yield();
			}
			flag = true;
		}
		void unlock() {
			flag = false;
		}
	private:
		volatile bool flag;
	};

private:

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
