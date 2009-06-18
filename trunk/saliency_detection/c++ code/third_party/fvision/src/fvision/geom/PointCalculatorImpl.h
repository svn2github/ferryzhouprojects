#ifndef FVISION_POINT_CALCULATOR_IMPL_H_
#define FVISION_POINT_CALCULATOR_IMPL_H_

#include "PointCalculator.h"

namespace fvision {

class DltPointCalculator : public PointCalculator {
public:
	CvPoint3D64f compute(const CameraPointSamples& samples);
};

class RANSAC_PointCalculator : public Ransac<CvPoint3D64f, CameraPointSample> {
public:
	RANSAC_PointCalculator(PointCalculator* pmc, int modelSampleSize, PointErrorCalculator* pec) 
		: Ransac(pmc, modelSampleSize, pec) {
	}

	static RANSAC_PointCalculator* create(double threshold, int modelSampleSize = 2) {
		PointGeometricErrorCalculator* pec = new PointGeometricErrorCalculator();
		pec->setThreshold(threshold);
		return new RANSAC_PointCalculator(new DltPointCalculator(), modelSampleSize, pec);
	}

public:
	~RANSAC_PointCalculator(void) {}
};

}
#endif // FVISION_POINT_CALCULATOR_IMPL_H_
