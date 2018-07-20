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
#include <stdlib.h>

namespace xpcc {

class Terminal {

public:
	Terminal(xpcc::IOStream &stream) : stream(stream) {}

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

	int arg(uint8_t id) {
		return atol(arglist[id]);
	}

	void printf(const char *fmt, ...) {
		va_list ap;
		va_start(ap, fmt);
		stream.vprintf(fmt, ap);
		va_end(ap);
	}

	bool readChar();

protected:
	virtual void handleCommand(uint8_t nargs, char* argv[]) = 0;

	xpcc::IOStream& stream;
	const char* prompt = "xpcc> ";
	bool echo = true;

private:
	char buffer[64];
	uint8_t pos = 0;
	char* arglist[6];

	void parse();
};

}

#endif /* TERMINAL_HPP_ */
