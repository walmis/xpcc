/*
 * terminal.hpp
 *
 *  Created on: Nov 8, 2013
 *      Author: walmis
 */

#ifndef TERMINAL_HPP_
#define TERMINAL_HPP_

#include <xpcc/architecture.hpp>
#include <xpcc/processing.hpp>

namespace xpcc {

class Terminal : xpcc::TickerTask {

public:
	Terminal(xpcc::IODevice &device) : device(device) {}


protected:
	bool cmp(char* cmd, const char* value) {
		return strcmp(cmd, value) == 0;
	}

	virtual void handleCommand(uint8_t nargs, char* argv[]) = 0;

	int to_int(const char *p);
	float toFloat(const char* c);

	xpcc::IODevice& device;

protected:
	virtual void handleTick() override;

private:
	char buffer[32];
	uint8_t pos = 0;

	void parse();
};

}

#endif /* TERMINAL_HPP_ */
