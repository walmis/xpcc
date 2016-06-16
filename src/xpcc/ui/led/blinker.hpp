#pragma once

#include <xpcc/processing.hpp>

namespace xpcc {
namespace ui
{


template <typename Pin, int deadtime = 20>
class Blinker {

public:

	Blinker() {
		t.stop();
		dead = false;
	}

	void blink(int time = 25) {
		if(!t.isActive()) {
			if(!dead) {
				Pin::set(1);
				t.restart(time);
			}
		}
	}

	void run() {
		if(t.isExpired() && t.isActive()) {
			if(!dead) {
				Pin::set(0);
				dead = true;
				t.restart(deadtime);
			} else {
				dead = false;
				t.stop();
			}
		}
	}

	volatile bool dead;
	xpcc::Timeout<> t;
};

}
}
