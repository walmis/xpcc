/*
 * r_mutex.hpp
 *
 *  Created on: Aug 30, 2017
 *      Author: walmis
 */

#ifndef SRC_XPCC_PROCESSING_RTOSABSTRACTION_CHIBIRTOS_R_MUTEX_HPP_
#define SRC_XPCC_PROCESSING_RTOSABSTRACTION_CHIBIRTOS_R_MUTEX_HPP_

/* Recursive mutex */
#include <stdint.h>
namespace xpcc {
	class RMutex {
	public:
		RMutex();
		~RMutex();

		void lock();
		bool lock(uint32_t timeout);
		bool try_lock();

		void unlock();
	private:
		void* data;
	};
}

#endif /* SRC_XPCC_PROCESSING_RTOSABSTRACTION_CHIBIRTOS_R_MUTEX_HPP_ */
