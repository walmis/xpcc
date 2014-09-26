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

	int toInt(const char *p);
	float toFloat(const char* c);

	inline bool cmp(char* cmd, const char* value) {
		return strcmp(cmd, value) == 0;
	}

protected:
	virtual void handleCommand(uint8_t nargs, char* argv[]) = 0;

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
