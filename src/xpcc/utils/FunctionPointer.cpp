/*
 * FunctionPointer.cpp
 *
 *  Created on: Oct 9, 2013
 *      Author: walmis
 */

#include "FunctionPointer.h"

namespace xpcc {

FunctionPointer::FunctionPointer(void (*function)(void)) {
    attach(function);
}

void FunctionPointer::attach(void (*function)(void)) {
    _function = function;
    _object = 0;
}

void FunctionPointer::call(void) {
    if (_function) {
        _function();
    } else if (_object) {
        _membercaller(_object, _member);
    }
}

void FunctionPointer::operator ()(void) {
    call();
}

} /* namespace xpcc */
