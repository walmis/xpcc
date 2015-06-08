#include "event.hpp"


void xpcc::Event::wait() {
	sem.take_nonblocking();
	sem.take();
}


void xpcc::Event::signal() {
	sem.give();
}
/*
 * event.cpp
 *
 *  Created on: May 28, 2015
 *      Author: walmis
 */




