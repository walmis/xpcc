// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2011, Roboterclub Aachen e.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Roboterclub Aachen e.V. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
// ----------------------------------------------------------------------------
#include <stdio.h>		// snprintf()
#include <stdlib.h>
#include <math.h>

#include <xpcc/utils/arithmetic_traits.hpp>
#include <xpcc/utils/template_metaprogramming.hpp>

#include "iostream.hpp"

inline void reverse(char str[], int length) {
	int start = 0;
	int end = length - 1;
	while (start < end) {
		char tmp = *(str + start);

		*(str + start) = *(str + end);
		*(str + end) = tmp;

		start++;
		end--;
	}
}

// Implementation of itoa()
inline uint8_t itoa(int num, char* str) {
	int i = 0;

	/* Handle 0 explicitely, otherwise empty string is printed for 0 */
	if (num == 0) {
		str[i++] = '0';
		return 1;
	}

	if (num < 0) {
		num = -num;
	}

	// Process individual digits
	while (num != 0) {
		int rem = num % 10;
		str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num = num / 10;
	}

	// Reverse the string
	reverse(str, i);

	return i;
}

void xpccFloat(char* str, float value, int precision) {
	float v;
	char *ptr = &str[0];

	if (precision >= 0) {
		int pow = 1;
		for (int i = 0; i < precision; i++) {
			pow *= 10;
		}

		value = roundf(value * pow) / pow;
	}

	if (value < 0) {
		v = -value;
		*ptr++ = '-';
	} else {
		v = value;
	}

	int32_t ep = 0;
	if (v != 0 && precision == -1) {
		while (v < 1.f) {
			v *= 10;
			ep -= 1;
		}

		while (v > 10) {
			v *= 0.1f;
			ep += 1;
		}
	}

	int digits = 6;
	if (precision >= 0) {
		digits = precision;
	}

	for (uint32_t i = 0; i < digits + 1; ++i) {
		int num = (int) (v);

		if (num > 9) {
			ptr += itoa(num, ptr);

		} else {
			*ptr++ = (num + '0');
		}

		if (i == 0) {
			*ptr++ = '.';
		}

		// next digit
		v = (v - num) * 10;
	}

	if (precision < 0) {
		*ptr++ = 'e';

		if (ep < 0) {
			ep = -ep;
			*ptr++ = '-';
		} else {
			*ptr++ = '+';
		}
		if (ep < 10) {
			*ptr++ = '0';
		}
	}
	if (precision < 0) {
		itoa(ep, ptr);
	}

	*ptr++ = '\0';	// End of string

}

//void xpcc::IOStream::writeFloat(float value, int precision) {
//	char str[20]; // +1 for '\0'
//	xpccFloat(str, value, precision);
//
//	this->device->write(str);
//}

// ----------------------------------------------------------------------------
//#if !defined(XPCC__CPU_AVR)
//void xpcc::IOStream::writeDouble(const double& value) {
////#if defined(XPCC__CPU_CORTEX_M3)
////	// TODO do this better
////	writeFloat(static_cast<float>(value));
////#else
////	// hard coded for 2.22507e-308
////	char str[13 + 1];// +1 for '\0'
////	snprintf(str, sizeof(str), "%.5e", value);
////	this->device->write(str);
////#endif
//}
//#endif
