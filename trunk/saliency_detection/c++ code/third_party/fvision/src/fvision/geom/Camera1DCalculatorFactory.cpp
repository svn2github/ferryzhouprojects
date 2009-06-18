#include "Camera1DCalculatorFactory.h"

#include "Camera1DCalculatorImpl.h"

namespace fvision {

Camera1DCalculator* Camera1DCalculatorFactory::create(const Configuration& configuration) {

	Camera1DCalculator* camera1DCalculator = NULL;
	if(configuration.calibrated) {
		camera1DCalculator = new CalibratedCamera1DCalculator(configuration.focalLength, configuration.principalPoint);
	} else {
		camera1DCalculator = new UnCalibratedCamera1DCalculator();
	}

	if (configuration.ransac) {
		int sampleSize = configuration.calibrated ? 5 : 6;
		Camera1DErrorCalculator* cec = new Camera1DErrorCalculator();
		cec->setThreshold(configuration.ransacErrorThreshold);
		camera1DCalculator = new RANSAC_Camera1DCalculator(camera1DCalculator, sampleSize, cec);
	}

	return camera1DCalculator;
}
}