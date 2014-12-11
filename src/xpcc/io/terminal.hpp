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

class Terminal : public xpcc::TickerTask {

public:
	Terminal(xpcc::IODevice &device) : device(device) {}

	int toInt(const char *p);
	float toFloat(const char* c);

	inline bool cmp(char* cmd, const char* value) {
		return strcmp(cmd, value) == 0;
	}

	bool cmd(uint8_t arg, const char* c) {
		return cmp(arglist[arg], c);
	}

	bool cmd(uint8_t arg, int c) {
		return atol(arglist[arg]) == c;
	}

protected:
	virtual void handleCommand(uint8_t nargs, char* argv[]) = 0;

	xpcc::IODevice& device;

protected:
	virtual void handleTick() override;

private:
	char buffer[32];
	uint8_t pos = 0;
	char* arglist[6];

	void parse();
};

}

#endif /* TERMINAL_HPP_ */
