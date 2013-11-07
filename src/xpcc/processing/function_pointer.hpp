/*
 * FunctionPointer.h
 *
 *  Created on: Oct 9, 2013
 *      Author: walmis
 */

#ifndef FUNCTIONPOINTER_H_
#define FUNCTIONPOINTER_H_

#include <functional>
#include <string.h>

namespace xpcc {
 
  
/**
 * \brief FunctionPointer
 *  A class for storing and calling a pointer to a static or member void function
 * 
 * \ingroup	workflow
 */

class FunctionPointer {
public:

	/** Create a FunctionPointer, attaching a static function
	 *
	 *  @param function The void static function to attach (default is none)
	 */
	FunctionPointer(std::function<void()> f = 0) {
		_function = f;
	}

	void call(void) {
	    if (_function) {
	        _function();
	    }
	}

	void operator ()(void) {
	    call();
	}

	/** Create a FunctionPointer, attaching a member function
	 *
	 *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
	 *  @param function The address of the void member function to attach
	 */
	template<typename T>
	FunctionPointer(T *object, void (T::*member)(void)) {
		attach(object, member);
	}

	inline void attach(FunctionPointer* other) {
		attach(other, &FunctionPointer::call);
	}
	
	/** Attach a static function
	 *
	 *  @param function The void static function to attach (default is none)
	 */
	void attach(std::function<void()> func) {
		_function = func;
	}

	/** Attach a member function
	 *
	 *  @param object The object pointer to invoke the member function on (i.e. the this pointer)
	 *  @param function The address of the void member function to attach
	 */
	template<typename T>
	void attach(T *object, void (T::*member)()) {
	    _function = std::bind(member, object);
	}


	std::function<void()> get_function() const {
		return _function;
	}


private:

	std::function<void()> _function;
};


} /* namespace xpcc */

#endif /* FUNCTIONPOINTER_H_ */
