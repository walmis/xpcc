/*
 * terminal.hpp
 *
 *  Created on: Nov 8, 2013
 *      Author: walmis
 */

#ifndef TERMINAL_HPP_
#define TERMINAL_HPP_

#include <xpcc/architecture.hpp>

namespace xpcc {

class Terminal : xpcc::TickerTask {

public:
	Terminal(xpcc::IODevice &device) : device(device) {}


protected:
	bool cmp(char* cmd, char* value) {
		return strcmp(cmd, value) == 0;
	}

	virtual void handleCommand(uint8_t nargs, char* argv[]) = 0;

	int to_int(char *p);
	bool float_scan(const wchar_t* wcs, float* val);

private:
	void handleTick() override;

	char buffer[32];
	uint8_t pos = 0;

	xpcc::IODevice& device;

	void parse();
};

}

#endif /* TERMINAL_HPP_ */
