#pragma once

#include "Camera1DCalculator.h"
#include <fvision/utils/cvmat_utils.h>

namespace fvision {

class CalibratedCamera1DCalculator : public Camera1DCalculator {
public:
	CalibratedCamera1DCalculator(double focalLength, double principalPoint);
	~CalibratedCamera1DCalculator();
public:
	Camera1D compute(const Projection1DSamples& samples);

private:
	double focalLength;
	double principalPoint;
};

class UnCalibratedCamera1DCalculator : public Camera1DCalculator {
public:
	UnCalibratedCamera1DCalculator();
	~UnCalibratedCamera1DCalculator();
public:
	Camera1D compute(const Projection1DSamples& samples);

private:
	CvMat* I;
};

class RANSAC_Camera1DCalculator : public Ransac<Camera1D, Projection1DSample> {
public:
	RANSAC_Camera1DCalculator(Camera1DCalculator* pcmc, int modelSampleSize, Camera1DErrorCalculator* pcec) 
		: Ransac(pcmc, modelSampleSize, pcec) {
	}

	/** create a new instance
	*/
	static RANSAC_Camera1DCalculator* createCalibrated(double focalLength, double principalPoint, double threshold, int modelSampleSize = 6) {
		Camera1DErrorCalculator* pcec = new Camera1DErrorCalculator();
		pcec->setThreshold(threshold);
		return new RANSAC_Camera1DCalculator(new CalibratedCamera1DCalculator(focalLength, principalPoint), modelSampleSize, pcec);
	}

	/** create a new instance assuming focalLength = 1 and principalPoint = 0.
	*/
	static RANSAC_Camera1DCalculator* createCalibrated(double threshold, int modelSampleSize = 6) {
		RANSAC_Camera1DCalculator* instance = createCalibrated(1, 0, threshold, modelSampleSize);
		return instance;
	}

public:
	~RANSAC_Camera1DCalculator(void) {}
};
}