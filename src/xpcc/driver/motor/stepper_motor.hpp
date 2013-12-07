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

enum DriveMode {
	FULL_STEP = 0,
	HALF_STEP = 1
};

template <typename Nibble>
class StepperMotor : TickerTask{
public:

	StepperMotor() {
		Nibble::setOutput();
		Nibble::write(0);

		moveSteps = 0;
		position = 0;
		mode = FULL_STEP;
	}

	void moveTo(int position) {
		move(position - getStepPosition());
	}

	void move(int steps) {
		if(mode == HALF_STEP) {
			steps *= 2;
		}

		moveSteps += steps;
		if(steps < 0) {
			step(REVERSE);
		} else {
			step(FORWARD);
		}
	}

	void setMode(DriveMode mode) {
		this->mode = mode;
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
		return (mode == HALF_STEP) ? position/2 : position;
	}

	void stop() {
		moveSteps = 0;
	}

	void wait() {
		while(t.isActive()) {
			run();
		}
	}

	void run() {
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

protected:
	void step(StepperDirection dir) {
		if(mode != HALF_STEP) {
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
		} else {
			if(dir == FORWARD) {
				position++;
				state++;
				state &= 7;
			} else {
				position--;
				if(state == 0)
					state = 7;
				else
					state--;
			}
		}

		if(mode == FULL_STEP) {
			switch (state) {
			case 0:
				Nibble::write(0b0011);
				break;
			case 1:
				Nibble::write(0b0110);
				break;
			case 2:
				Nibble::write(0b1100);
				break;
			case 3:
				Nibble::write(0b1001);
				break;
			}
		} else if(mode == HALF_STEP) {
			switch (state) {
			case 0:
				Nibble::write(0b0011);
				break;
			case 1:
				Nibble::write(0b0010);
				break;
			case 2:
				Nibble::write(0b0110);
				break;
			case 3:
				Nibble::write(0b0100);
				break;
			case 4:
				Nibble::write(0b1100);
				break;
			case 5:
				Nibble::write(0b1000);
				break;
			case 6:
				Nibble::write(0b1001);
				break;
			case 7:
				Nibble::write(0b0001);
				break;
			}
		}

		t.restart(delay);
	}

	void handleTick() {
		run();
	}

	uint8_t delay = 10;
	int moveSteps;

	int position;
	uint8_t state = 0;

	DriveMode mode;

	Timeout<> t;
};

}

#endif /* STEPPER_HPP_ */
