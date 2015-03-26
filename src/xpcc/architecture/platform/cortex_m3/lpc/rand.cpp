#include "rand.hpp"
extern uint8_t __heap_start;
extern uint8_t __heap_end;
extern uint8_t __stack_start;
extern uint8_t __stack_end;

void xpcc::Random::seed(uint32_t seed) {
	uint32_t sum = 0;

	for(uint32_t *i = (uint32_t*)&__stack_start; i < (uint32_t*)&__stack_end; i++) {
		sum ^= *i;
	}

	srand(sum);
}
