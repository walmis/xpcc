/*
 * interrupt_wrapper.hpp
 *
 *  Created on: May 28, 2015
 *      Author: walmis
 */

#ifndef SRC_XPCC_PROCESSING_SIMPLETASKER_INTERRUPT_WRAPPER_HPP_
#define SRC_XPCC_PROCESSING_SIMPLETASKER_INTERRUPT_WRAPPER_HPP_

namespace xpcc {

class IRQWrapper {
public:
	//prologue
#if __ARM_ARCH==6
	IRQWrapper(void* lr = __builtin_return_address(0));
#else
	IRQWrapper();
#endif
	//epilogue
	~IRQWrapper();
private:
	void* data;
};
}


#endif /* SRC_XPCC_PROCESSING_SIMPLETASKER_INTERRUPT_WRAPPER_HPP_ */
