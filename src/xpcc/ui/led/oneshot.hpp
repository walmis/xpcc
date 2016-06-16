#pragma once
#include <xpcc/processing/timer_registry.hpp>
#include <xpcc/processing/timeout.hpp>
#include <functional>

namespace xpcc {
namespace ui {

template <typename LED>
class OneShot {
public:
	xpcc::Timeout<> t;
	int timer;

	void trigger(uint32_t onTime) {
		LED::set();
		t.restart(onTime);
		if(!timer) {
			timer = TimerRegistry::registerTimer(std::bind(&OneShot::run, this));
		}
	}

	void run() {
		if(t.isExpired()) {
			LED::reset();
			TimerRegistry::unregisterTimer(timer);
			timer = 0;
		}
	}
};

}
}
