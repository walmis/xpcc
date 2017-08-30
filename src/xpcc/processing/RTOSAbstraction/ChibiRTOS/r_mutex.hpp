/*
 * r_mutex.hpp
 *
 *  Created on: Aug 30, 2017
 *      Author: walmis
 */

#ifndef SRC_XPCC_PROCESSING_RTOSABSTRACTION_CHIBIRTOS_R_MUTEX_HPP_
#define SRC_XPCC_PROCESSING_RTOSABSTRACTION_CHIBIRTOS_R_MUTEX_HPP_

/* Recursive mutex */

class RMutex {
public:
	RMutex();
	~RMutex();

	void lock();
	void unlock();
	bool try_lock();

private:
	void* data;
};


#endif /* SRC_XPCC_PROCESSING_RTOSABSTRACTION_CHIBIRTOS_R_MUTEX_HPP_ */
