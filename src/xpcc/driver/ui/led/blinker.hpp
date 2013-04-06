#pragma once

#include <xpcc/workflow.hpp>

namespace xpcc {

template <typename Pin>
class Blinker : xpcc::TickerTask {

public:

	Blinker() {
		t.stop();
	}

	void blink(int time = 10) {
		Pin::set(1);
		t.restart(time);
	}

	void handleTick() override {
		if(t.isExpired()) {
			Pin::set(0);
		}
	}

	xpcc::Timeout<> t;
};

}
