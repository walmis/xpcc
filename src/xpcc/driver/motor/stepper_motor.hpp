/*
 * stepper.hpp
 *
 *  Created on: Nov 20, 2013
 *      Author: walmis
 */

#ifndef STEPPER_HPP_
#define STEPPER_HPP_

#include <xpcc/processing.hpp>

namespace xpcc {

enum StepperDirection {
	FORWARD,
	REVERSE
};

template <typename Nibble>
class StepperMotor : TickerTask{
public:

	StepperMotor() {
		Nibble::setOutput();
		Nibble::write(0);

		moveSteps = 0;
	}

	void step(StepperDirection dir) {
		if(dir == FORWARD) {
			position++;
			state++;
			state &= 3;
		} else {
			position--;
			if(state == 0)
				state = 3;
			else
				state--;
		}

		//XPCC_LOG_DEBUG .printf("state %d\n", state);
//		switch (state) {
//		case 0:    // 1010
//			Nibble::write(0b0001);
//			break;
//
//		case 1:    // 0110
//			Nibble::write(0b0010);
//			break;
//
//		case 2:    //0101
//			Nibble::write(0b0100);
//			break;
//
//		case 3:    //1001
//			Nibble::write(0b1000);
//			break;
//		}
		switch (state) {
		case 0:    // 1010
			Nibble::write(0b0011);
			break;

		case 1:    // 0110
			Nibble::write(0b0110);
			break;

		case 2:    //0101
			Nibble::write(0b1100);
			break;

		case 3:    //1001
			Nibble::write(0b1001);
			break;
		}
		t.restart(delay);
	}

	void move(int steps) {
		moveSteps = steps;
		if(steps < 0) {
			step(REVERSE);
		} else {
			step(FORWARD);
		}
	}

	void setSpeed(uint8_t delay) {
		this->delay = delay;
	}

	void resetStepPosition() {
		position = 0;
	}

	void setStepPosition(int position) {
		this->position = position;
	}

	int getStepPosition() {
		return position;
	}

	void stop() {
		moveSteps = 0;
	}

protected:
	void handleTick() {
		if(t.isActive() && t.isExpired()) {

			if(moveSteps != 0) {
				if(moveSteps < 0) {
					step(REVERSE);
					moveSteps++;

				} else {
					step(FORWARD);
					moveSteps--;
				}

			} else {

				Nibble::write(0);
				t.stop();
			}
		}
	}

	uint8_t delay = 10;
	int moveSteps;

	int position;
	uint8_t state = 0;

	Timeout<> t;
};

}

#endif /* STEPPER_HPP_ */
