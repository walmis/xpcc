/*
 * interrupt_wrapper.hpp
 *
 *  Created on: May 28, 2015
 *      Author: walmis
 */

#ifndef SRC_XPCC_PROCESSING_SIMPLETASKER_INTERRUPT_WRAPPER_HPP_
#define SRC_XPCC_PROCESSING_SIMPLETASKER_INTERRUPT_WRAPPER_HPP_

namespace xpcc {

//default one does nothing

class IRQWrapper {
public:
	//prologue
	IRQWrapper() {}
	//epilogue
	~IRQWrapper() {}
};
}


#endif /* SRC_XPCC_PROCESSING_SIMPLETASKER_INTERRUPT_WRAPPER_HPP_ */
