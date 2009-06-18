#pragma once

#include "CameraCalculator.h"
#include <fvision/utils/CvMatWrapper.h>

namespace fvision {

/**
 * Factory for creating CameraCalculator
 * @see CameraCalculator 
 */
class CameraCalculatorFactory {
public:
	class Configuration {
	public:
		Configuration() {
			ransac = true;
			calibrated = false;
			ransacErrorThreshold = 1.5; //pixels
		}

	public:
		bool ransac;
		double ransacErrorThreshold;
		bool calibrated;
		CvMatWrapper K;
		CvMatWrapper k;
	};

public:
	CameraCalculator* create(const Configuration& configuration);
};

}