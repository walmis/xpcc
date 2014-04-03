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
	HALF_STEP = 1,
	WAVE_DRIVE = 2
};

template <typename Nibble>
class StepperMotor : TickerTask{
public:

	StepperMotor() {
		Nibble::setOutput();
		Nibble::write(0);

		delay = 10;
		idleTimeout = 1;

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

		if(!stepTimer.isActive()) {
			stepTimer.restart(0);
		}
	}

	void setMode(DriveMode mode) {
		this->mode = mode;
	}

	void setSpeed(uint8_t delay) {
		this->delay = delay;
	}

	void setIdleTimeout(uint8_t timeoutMs) {
		this->idleTimeout = timeoutMs;
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
		while(isBusy());
	}

	bool isBusy() {
		run();
		return moveSteps != 0 && stepTimer.isActive();
	}

	void run() {
		if(stepTimer.isActive() && stepTimer.isExpired()) {
			if(moveSteps != 0) {
				if(moveSteps < 0) {
					step(REVERSE);
					moveSteps++;

				} else {
					step(FORWARD);
					moveSteps--;
				}
			} else {
				idleTimer.restart(idleTimeout);
				stepTimer.stop();
			}
		}

		if(idleTimer.isActive() && idleTimer.isExpired()) {
			setOutput(0);
			idleTimer.stop();
		}
	}


protected:

	virtual void setOutput(uint8_t bits) {
		Nibble::write(bits);
	}

	void step(StepperDirection dir) {
		switch(mode) {
		case FULL_STEP:
		case WAVE_DRIVE:

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
			break;

		case HALF_STEP:
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
			break;
		}

		//XPCC_LOG_DEBUG .printf("state %d\n", state);
		switch(mode) {
		case WAVE_DRIVE:
			switch (state) {
				case 0:
					setOutput(0b1000);
					break;
				case 1:
					setOutput(0b0001);
					break;
				case 2:
					setOutput(0b0100);
					break;
				case 3:
					setOutput(0b0010);
					break;
				}
			break;

		case FULL_STEP:
			switch (state) {
				case 0:
					setOutput(0b1010);
					break;
				case 1:
					setOutput(0b1001);
					break;
				case 2:
					setOutput(0b0101);
					break;
				case 3:
					setOutput(0b0110);
					break;
				}
			break;
			case HALF_STEP:

				switch (state) {
				case 0:
					setOutput(0b1010);
					break;
				case 1:
					setOutput(0b1000);
					break;
				case 2:
					setOutput(0b1001);
					break;
				case 3:
					setOutput(0b0001);
					break;
				case 4:
					setOutput(0b0101);
					break;
				case 5:
					setOutput(0b0100);
					break;
				case 6:
					setOutput(0b0110);
					break;
				case 7:
					setOutput(0b0010);
					break;
				}
		}

		stepTimer.restart(delay);
	}

	void handleTick() {
		run();
	}

	uint8_t idleTimeout;
	uint8_t delay;
	int moveSteps;

	int position;
	uint8_t state = 0;

	DriveMode mode;

	Timeout<> stepTimer;
	Timeout<> idleTimer;
};

}

#endif /* STEPPER_HPP_ */
