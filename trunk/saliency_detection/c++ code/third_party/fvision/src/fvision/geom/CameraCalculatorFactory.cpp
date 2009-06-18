#include "CameraCalculatorFactory.h"

#include "CameraCalculatorImpl.h"

namespace fvision {

CameraCalculator* CameraCalculatorFactory::create(const Configuration& configuration) {
	CameraCalculator* cameraCalculator = NULL;
	if(configuration.calibrated) {
		cameraCalculator = new CalibratedCameraCalculator(configuration.K.getMat(), configuration.k.getMat());
	} else {
		cameraCalculator = new UnCalibratedCameraCalculator();
	}

	if (configuration.ransac) {
		int sampleSize = configuration.calibrated ? 6 : 12;
		CameraErrorCalculator* cec = new CameraErrorCalculator();
		cec->setThreshold(configuration.ransacErrorThreshold);
		//if (configuration.calibrated) {
		//	cec->setThreshold(abs(configuration.ransacErrorThreshold / cvmGet(configuration.K.getMat(), 0, 0)));
		//}
		cameraCalculator = new RANSAC_CameraCalculator(cameraCalculator, sampleSize, cec);
	}

	return cameraCalculator;
}

}