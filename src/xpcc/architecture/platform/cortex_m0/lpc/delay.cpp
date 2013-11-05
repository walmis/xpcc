/*
 * delay.c
 *
 *  Created on: Jan 31, 2013
 *      Author: walmis
 */
#include <xpcc/architecture.hpp>
#include <xpcc/processing.hpp>.hpp>

inline uint32_t divide_by_1000000(uint32_t n) {
    const uint32_t reciprocal = ((1<<24) + 1000000 - 1) / 1000000;
    return (n * reciprocal) >> 24;
}

extern "C" {
	void _delay_us(uint32_t us) {
		if(us > 1000) {
			_delay_ms(us/1000);
		}
		int32_t ticks = us * divide_by_1000000(SystemCoreClock);
		if(ticks < 0) return;
		int32_t cur = SysTick->VAL;

		while((cur - SysTick->VAL) < ticks);
	}

	void _delay_ms(uint32_t time) {
		xpcc::Timeout<> t(time);

		while(!t.isExpired()) {};


	}
}
