/*
 * rand.hpp
 *
 *  Created on: Feb 23, 2013
 *      Author: walmis
 */

#ifndef RAND_HPP_
#define RAND_HPP_

#include <stdlib.h>

extern uint8_t __heap_start;
extern uint8_t __heap_end;
extern uint8_t __stack_start;
extern uint8_t __stack_end;

namespace xpcc {


class Random {
public:
	static void seed(uint32_t seed = 0) {
		uint32_t sum = 0;

		for(uint32_t *i = (uint32_t*)&__stack_start; i < (uint32_t*)&__stack_end; i++) {
			sum ^= *i;
		}

		srand(sum);
	}
	static uint32_t random() {
		return rand();
	}

};

}

#endif /* RAND_HPP_ */
