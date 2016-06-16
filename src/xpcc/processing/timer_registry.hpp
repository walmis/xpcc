//Author: Valmantas Paliksa <walmis@gmail.com>, 2016.05
//Licence: GNU GPL

#pragma once

#include <utility>
#include <xpcc/processing/function.hpp>
#include <xpcc/container/linked_list.hpp>


class TimerRegistry
{
public:
        //this constructor is platform dependent
        //it should attach _timerTick function to a millisecond interrupt
	TimerRegistry();
        
        static void _timerTick();

	static int registerTimer(xpcc::function<void()> callback);
	static void unregisterTimer(int id);
	static TimerRegistry* instance();
        
private:
    
    static xpcc::LinkedList<std::pair<int,xpcc::function<void()>>> list;
    static uint32_t counter;
    
};
