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
	if(prompt) {
		stream.write('\r');
		stream.write((char*)prompt);
	}

	memset(buffer, 0, sizeof(buffer));

}

bool Terminal::readChar() {
	int16_t c;
	if ((c = stream.read()) > 0) {

		if(c == '\b' || c == 127) { //backspace
			if(pos > 0) {
				pos--;
	            stream.write('\b');
	            stream.write(' ');
	            stream.write('\b');
			}
		} else {
			if (c == '\n' || c == '\r') {
				//remove the newline character
				buffer[pos] = 0;
				if(echo) {
					stream.write('\r');
					stream.write('\n');
				}

				parse();
				pos = 0;
				return true;
			} else {
				if(!isprint(c)) {
					return true;
				} else {
					buffer[pos] = c;
					pos++;
					pos %= sizeof(buffer);
					if(echo) {
						stream.write(c);
					}
				}
			}

		}
		return true;
	}
	return false;
}

}

