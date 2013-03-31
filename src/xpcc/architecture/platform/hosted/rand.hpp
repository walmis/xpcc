/*
 * rand.hpp
 *
 *  Created on: Mar 25, 2013
 *      Author: walmis
 */

#ifndef RAND_HPP_
#define RAND_HPP_

#include <sys/time.h>
#include <cstdlib>

namespace xpcc {

	class Random {
	public:
		static void seed(uint32_t seed = 0) {
			if(seed == 0) {
				timeval time;
				gettimeofday(&time, NULL);
				seed = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;
			}
			srand(seed);
		}
		static uint32_t random() {
			return rand();
		}

	};


}


#endif /* RAND_HPP_ */
