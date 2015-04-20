/*
 * service.hpp
 *
 *  Created on: Mar 30, 2015
 *      Author: walmis
 */

#ifndef SRC_XPCC_PROCESSING_SERVICE_HPP_
#define SRC_XPCC_PROCESSING_SERVICE_HPP_

#include <stdint.h>
#include <xpcc/processing/function.hpp>

namespace xpcc {

	void registerServiceCallHandler(uint32_t id, xpcc::function<void()> callback);
	void serviceCall(uint32_t id);

}



#endif /* SRC_XPCC_PROCESSING_SERVICE_HPP_ */
