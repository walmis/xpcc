/*
 * tickertask2.cpp
 *
 *  Created on: Apr 2, 2013
 *      Author: walmis
 */

#include <xpcc/processing.hpp>

#include <xpcc/architecture.hpp>
#include <xpcc/architecture/driver/atomic.hpp>

using namespace xpcc;

#define MAIN_RETURN 0xFFFFFFF9  //Tells the handler to return using the MSP
#define THREAD_RETURN 0xFFFFFFFD //Tells the handler to return using the PSP

//This defines the stack frame that is saved  by the hardware
typedef struct {
  uint32_t r0;
  uint32_t r1;
  uint32_t r2;
  uint32_t r3;
  uint32_t r12;
  uint32_t lr;
  uint32_t pc;
  uint32_t psr;
} hw_stack_frame_t;

//This defines the stack frame that must be saved by the software
typedef struct {
  uint32_t r4;
  uint32_t r5;
  uint32_t r6;
  uint32_t r7;
  uint32_t r8;
  uint32_t r9;
  uint32_t r10;
  uint32_t r11;
} sw_stack_frame_t;

static CoopTask_Base* volatile currTask = 0;

//holds the context of the main thread
uint8_t main_context[sizeof(sw_stack_frame_t)];

inline void save_context(void){
  uint32_t scratch;
  asm volatile ("MRS %0, psp\n\t"
      "STMDB %0!, {r4-r11}\n\t"
      "MSR psp, %0\n\t"  : "=r" (scratch) );
}

//This loads the context from the PSP, the Cortex-M3 loads the other registers using hardware
inline void load_context(void){
  uint32_t scratch;
  asm volatile ("MRS %0, psp\n\t"
      "LDMFD %0!, {r4-r11}\n\t"
      "MSR psp, %0\n\t"  : "=r" (scratch) );
}

struct InitPSP {
	InitPSP() {
		__set_PSP((uint32_t)(main_context + sizeof(sw_stack_frame_t)));
		NVIC_SetPriority(PendSV_IRQn, 0xFF);
	}
} _initpsp;

CoopTask_Base::CoopTask_Base(void* stack, size_t stackSize) {
	xpcc::atomic::Lock lock;

	this->stacksize = stackSize;
	this->stack = stack;

	hw_stack_frame_t* process_frame;
	process_frame = (hw_stack_frame_t*) (((uint8_t*) (stack) + stackSize
			- sizeof(hw_stack_frame_t)));
	process_frame->r0 = (uint32_t) (this);
	process_frame->r1 = 0;
	process_frame->r2 = 0;
	process_frame->r3 = 0;
	process_frame->r12 = 0;
	process_frame->pc =
			(uint32_t) (reinterpret_cast<void*>(&CoopTask_Base::_thread));
	process_frame->lr = 0xFFFFFFFF;
	process_frame->psr = 0x21000000; //default PSR value
	//XPCC_LOG_DEBUG.printf("stack %x\n", stack);
	sp = (uint8_t*) (stack) + stackSize - sizeof(hw_stack_frame_t)
			- sizeof(sw_stack_frame_t);
}

void CoopTask_Base::_yield(uint16_t time_available) {
	//if yield is called from thread, issue a pendSV,
	//do nothing otherwise
	//XPCC_LOG_DEBUG .printf("yield %x\n", this);
	if(isInterruptContext()) return;

	if((__get_CONTROL() & 2) && !(flags & TickerTask::FLAG_YIELDING)) {
		flags |= TickerTask::FLAG_YIELDING;
		if(time_available) {
			sleep_timeout = xpcc::Clock::now() + time_available;
			flags |= TickerTask::FLAG_SLEEPING;
		}
		//issue pendSV
		SCB->ICSR = (1<<28);
	}

}

void CoopTask_Base::_handleTick() {

	if(!(flags & TickerTask::FLAG_SLEEPING)) {
		currTask = this;
		flags &= ~TickerTask::FLAG_YIELDING;
		__DMB();
		//issue pendSV
		SCB->ICSR = (1<<28);
	} else {
		if(xpcc::Clock::now() >= sleep_timeout) {
			flags &= ~TickerTask::FLAG_SLEEPING;
		}
	}

}

void* CoopTask_Base::contextSwitch(void* arg) {
	uint32_t* lr = (uint32_t*)arg;

	//current executing task has yielded the cpu
	//return to main thread for rescheduling
//	if(!currTask) {
//		__set_PSP((uint32_t)main_context);
//		*lr = MAIN_RETURN;
//		return 0;
//	}
	if(currTask->flags & TickerTask::FLAG_YIELDING) {
		//save the current stack pointer
		currTask->sp = (void*)__get_PSP();
		currTask->flags &= ~TickerTask::FLAG_YIELDING;

		//XPCC_LOG_DEBUG .printf("return to %x msp %x\n", currTask, __get_MSP());
		currTask = 0;
		//set new PSP to point to main thread's state values
		__set_PSP((uint32_t)main_context);
		*lr = MAIN_RETURN;
	} else {
		//XPCC_LOG_DEBUG .printf("load task %x psp %x\n", currTask, currTask->stackPtr);

		__set_PSP((uint32_t)currTask->sp);
		*lr = THREAD_RETURN;
	}
	return 0;
}

extern "C" __attribute__((naked)) void PendSV_Handler() {
	__disable_irq();
	uint32_t* lr;

	save_context();

	asm volatile("PUSH {lr};"
				 "MRS %0, MSP;"
				 : "=r"(lr));

	CoopTask_Base::contextSwitch(lr);

	//XPCC_LOG_DEBUG .printf("PendSV %x new psp %x\n", *lr, __get_PSP());

	load_context();
	__enable_irq();
	asm volatile("pop {pc}"); //pop value lr from stack to pc register
}
