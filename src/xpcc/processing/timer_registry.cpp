#include "timer_registry.hpp"
#include <utility>

#include <xpcc/architecture.hpp>

static TimerRegistry registry;

xpcc::LinkedList<std::pair<int,xpcc::function<void()>>> TimerRegistry::list;
uint32_t TimerRegistry::counter;

void TimerRegistry::_timerTick() {
        auto &list = registry.list;
    
        for(auto iter = list.begin(); iter != list.end(); ++iter) {
                if(iter->second) {
                        iter->second();
                }
        }
}

int TimerRegistry::registerTimer(xpcc::function<void()> callback) {
        counter++;

        auto p = std::make_pair(counter, callback);

        {
            xpcc::atomic::Lock l;
            list.append(p);
        }

        return counter;
}

void TimerRegistry::unregisterTimer(int id) {
        auto iter = list.begin();
        for(auto iter = list.begin(); iter != list.end(); ++iter) {
                if(iter->first == id) {
                        {
                                xpcc::atomic::Lock l;
                                list.remove(iter);
                        }

                }
        }
}
