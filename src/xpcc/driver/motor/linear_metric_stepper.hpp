/*
 * linear_metric_stepper.hpp
 *
 *  Created on: Nov 20, 2013
 *      Author: walmis
 */

#ifndef LINEAR_METRIC_STEPPER_HPP_
#define LINEAR_METRIC_STEPPER_HPP_


#include <math.h>
#include "stepper_motor.hpp"

namespace xpcc {

template <typename Nibble>
class MetricLinearStepper : public StepperMotor<Nibble> {

public:
	MetricLinearStepper(float stepSize) : stepSize(stepSize) {

	}

	float getPosition() {
		return this->getStepPosition() * stepSize;
	}

	void setPosition(float absPosition) {
		float diff = absPosition - getPosition();
		this->move((int)roundf(diff / stepSize));
	}


protected:
	float stepSize;

};
}

#endif /* LINEAR_METRIC_STEPPER_HPP_ */
