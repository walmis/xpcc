#pragma once

#include "../timer.hpp"

namespace xpcc {
namespace lpc11 {

template <int timerptr>
void Timer<timerptr>::enableCapturePins()
{
    if (TIMER == LPC_TMR16B0) {  /** CT16B0_CAP0 */
        IOCon::setPinFunc(0, 2, 2);

    } else if (TIMER == LPC_TMR16B1) {  /** CT16B1_CAP1 */
        IOCon::setPinFunc(0, 20, 1);

    } else if (TIMER == LPC_TMR32B0) {  /** CT32B0_CAP0 */
        IOCon::setPinFunc(0, 17, 2);

    } else if (TIMER == LPC_TMR32B1) {  /** CT32B1_CAP0 */
        IOCon::setPinFunc(0, 12, 3);
    }

}

template<int timerptr>
void Timer<timerptr>::assignMatchPins(MatchPins matchPins) {
	// 16-bit counter/timer 0 external Match Output
	if (TIMER == LPC_TMR16B0) {
		switch(matchPins) {
			case MatchPins::PINS_MAT0:
			IOCon::setPinFunc(0, 8, 2);
			break;
			case MatchPins::PINS_MAT1:
			IOCon::setPinFunc(0, 9, 2);
			break;
			case MatchPins::PINS_MAT2:
			IOCon::setPinFunc(0, 10, 3);
			break;
			case MatchPins::PINS_MAT3:
			break;
		}

		// 16-bit counter/timer 1 external Match Output
	} else
	if (TIMER == LPC_TMR16B1) {
		switch(matchPins) {
			case MatchPins::PINS_MAT0:
			IOCon::setPinFunc(1, 9, 1);
			break;
			case MatchPins::PINS_MAT1:
			IOCon::setPinFunc(0, 10, 2);
			break;
			case MatchPins::PINS_MAT2:
			case MatchPins::PINS_MAT3:
			break;
		}

		// 32-bit counter/timer 0 external Match Output
	} else
	if (TIMER == LPC_TMR32B0) {
		switch(matchPins) {
			case MatchPins::PINS_MAT0:
			IOCon::setPinFunc(1, 6, 2);
			break;
			case MatchPins::PINS_MAT1:
			IOCon::setPinFunc(1, 7, 2);
			break;
			case MatchPins::PINS_MAT2:
			IOCon::setPinFunc(0, 1, 2);
			break;
			case MatchPins::PINS_MAT3:
			IOCon::setPinFunc(0, 11, 3);
			break;
		}

		// 32-bit counter/timer 1 external Match Output
	} else
	if (TIMER == LPC_TMR32B1) {
		switch(matchPins) {
			case MatchPins::PINS_MAT0:
			IOCon::setPinFunc(1, 1, 3);
			break;
			case MatchPins::PINS_MAT1:
			IOCon::setPinFunc(1, 2, 3);
			break;
			case MatchPins::PINS_MAT2:
			IOCon::setPinFunc(1, 3, 3);
			break;
			case MatchPins::PINS_MAT3:
			IOCon::setPinFunc(1, 4, 2);
			break;
		}
	}
}

}
}
