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

// To convert a-f or A-F to a decimal number
static int chartoint(int c)
{
    char hex[] = "aAbBcCdDeEfF";
    int i;
    int result = 0;

    for(i = 0; result == 0 && hex[i] != '\0'; i++)
    {
        if(hex[i] == c)
        {
            result = 10 + (i / 2);
        }
    }

    return result;
}

static unsigned int htoi(const char s[])
{
    unsigned int result = 0;
    int i = 0;
    int proper = 1;
    int temp;

    //To take care of 0x and 0X added before the hex no.
    if(s[i] == '0')
    {
        ++i;
        if(s[i] == 'x' || s[i] == 'X')
        {
            ++i;
        }
    }

    while(proper && s[i] != '\0')
    {
        result = result * 16;
        if(s[i] >= '0' && s[i] <= '9')
        {
            result = result + (s[i] - '0');
        }
        else
        {
            temp = chartoint(s[i]);
            if(temp == 0)
            {
                proper = 0;
            }
            else
            {
                result = result + temp;
            }
        }

        ++i;
    }
    //If any character is not a proper hex no. ,  return 0
    if(!proper)
    {
        result = 0;
    }

    return result;
}

int Terminal::to_int(const char *p) {

	if(p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
		return htoi(p);
	}

	int k = 0;
	bool neg = false;
	if (*p == '-') {
		p++;
		neg = true;
	}

	while (*p) {
		k = (k << 3) + (k << 1) + (*p) - '0';
		p++;
	}
	if (neg)
		return -k;
	return k;
}

inline unsigned int ipow(int x, int n) {
    int r = 1;
    while (n--)
    r *= x;

    return r;
}

float Terminal::toFloat(const char* c) {
	char str[20];
	strncpy(str, c, 20);

	char* tok;
	tok = strtok(str, ".");

	int base = to_int(tok);

	tok = strtok(NULL, ".");
	if(tok) {
		uint8_t len = strlen(tok);

		float l = (float)to_int(tok) / ipow(10, len);
		l += base;
		return l;

	} else {
		return base;
	}
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

	if (device.read(buffer[pos])) {

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

