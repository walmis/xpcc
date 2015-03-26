/*
 * rand.hpp
 *
 *  Created on: Feb 23, 2013
 *      Author: walmis
 */

#ifndef RAND_HPP_
#define RAND_HPP_

#include <stdlib.h>
#include <stdint.h>

namespace xpcc {

class Random {
public:
	static void seed(uint32_t seed = 0);

	static uint32_t random() {
		return rand();
	}

};

}

#endif /* RAND_HPP_ */
