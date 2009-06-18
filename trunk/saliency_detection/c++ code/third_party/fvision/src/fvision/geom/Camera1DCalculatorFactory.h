#pragma once

#include "Camera1DCalculator.h"

namespace fvision {

class Camera1DCalculatorFactory {
public:
	class Configuration {
	public:
		Configuration() {
			ransac = true;
			calibrated = true;
			ransacErrorThreshold = 0.01; //pixels
		}

	public:
		bool ransac;
		double ransacErrorThreshold;
		bool calibrated;
		double focalLength;
		double principalPoint;
	};

public:
	Camera1DCalculator* create(const Configuration& configuration);
};

}