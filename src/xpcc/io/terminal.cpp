/*
 * terminal.cpp
 *
 *  Created on: Nov 12, 2013
 *      Author: walmis
 */

#include "terminal.hpp"
#include <math.h>
#include <ctype.h>

namespace xpcc {




int Terminal::toInt(const char *p) {
	return atoi(p);
}


float Terminal::toFloat(const char* c) {
	return atof(c);
}

void Terminal::parse() {
	//XPCC_LOG_DEBUG.printf(": %s\n", buffer);

	char* tok = strtok(buffer, " ");

	char* arglist[6];
	uint8_t nargs = 0;

	memset(arglist, 0, sizeof(char*) * 6);

	while (tok && nargs < 5) {
		arglist[nargs] = tok;
		tok = strtok(0, " ");
		nargs++;
	}

	handleCommand(nargs, arglist);

}

void Terminal::handleTick() {
	int16_t c;
	if ((c = device.read()) > 0) {
		buffer[pos] = c;
		if (buffer[pos] == '\n') {
			//remove the newline character
			buffer[pos] = 0;
			parse();
			pos = 0;
			return;
		} else {
			if(!isprint(buffer[pos])) {
				return;
			}
		}
		pos++;
		pos &= 31;
	}
}

}

