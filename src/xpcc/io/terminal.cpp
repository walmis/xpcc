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

	uint8_t nargs = 0;

	memset(arglist, 0, sizeof(char*) * 6);

	while (tok && nargs < 5) {
		arglist[nargs] = tok;
		tok = strtok(0, " ");
		nargs++;
	}

	handleCommand(nargs, arglist);

	device.write('\r');
	device.write("xpcc> ");

}

void Terminal::handleTick() {
	int16_t c;
	if ((c = device.read()) > 0) {

		if(c == '\b' || c == 127) { //backspace
			if(pos > 0) {
				pos--;
	            device.write('\b');
	            device.write(' ');
	            device.write('\b');
			}
		} else {
			if (c == '\n' || c == '\r') {
				//remove the newline character
				buffer[pos] = 0;

				device.write('\r');
				device.write('\n');

				parse();
				pos = 0;
				return;
			} else {
				if(!isprint(c)) {
					return;
				} else {
					buffer[pos] = c;
					pos++;
					pos &= 31;
					device.write(c);
				}
			}

		}
	}
}

}

