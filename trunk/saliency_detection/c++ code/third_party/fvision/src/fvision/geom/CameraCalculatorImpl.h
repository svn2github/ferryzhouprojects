#pragma once

#include "CameraCalculator.h"
#include <fvision/utils/cvmat_utils.h>

namespace fvision {

class CalibratedCameraCalculator : public CameraCalculator {
public:
	//default K = I, k = NULL
	CalibratedCameraCalculator();
	CalibratedCameraCalculator(const CvMat* K, const CvMat* k = NULL);
	~CalibratedCameraCalculator();
public:
	Camera compute(const ProjectionSamples& samples);

private:
	CvMat* K;
	CvMat* k;
};

class UnCalibratedCameraCalculator : public CameraCalculator {
public:
	UnCalibratedCameraCalculator();
	~UnCalibratedCameraCalculator();
public:
	Camera compute(const ProjectionSamples& samples);

private:
	CvMat* I;
};

class RANSAC_CameraCalculator : public Ransac<Camera, ProjectionSample> {
public:
	RANSAC_CameraCalculator(CameraCalculator* pcmc, int modelSampleSize, CameraErrorCalculator* pcec) 
		: Ransac(pcmc, modelSampleSize, pcec) {
	}

	/** create a new instance
	*/
	static RANSAC_CameraCalculator* createCalibrated(CvMat* K, CvMat* k, double threshold, int modelSampleSize = 6) {
		CameraErrorCalculator* pcec = new CameraErrorCalculator();
		pcec->setThreshold(threshold);
		return new RANSAC_CameraCalculator(new CalibratedCameraCalculator(K, k), modelSampleSize, pcec);
	}

	/** create a new instance assuming K = I and k = NULL.
	*/
	static RANSAC_CameraCalculator* createCalibrated(double threshold, int modelSampleSize = 6) {
		CvMat* K = getIdentity();
		RANSAC_CameraCalculator* instance = createCalibrated(K, NULL, threshold, modelSampleSize);
		cvReleaseMat(&K);
		return instance;
	}

public:
	~RANSAC_CameraCalculator(void) {}
};

}