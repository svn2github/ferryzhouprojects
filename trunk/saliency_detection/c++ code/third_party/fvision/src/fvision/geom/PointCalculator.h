#ifndef FVISION_POINT_CALCULATOR_H_
#define FVISION_POINT_CALCULATOR_H_

#include <fvision/algorithm/ransac.h>
#include <fvision/utils/CvMatWrapper.h>
#include <fvision/utils/model_evaluation_template.h>

namespace fvision {

typedef std::pair<CvMatWrapper, CvPoint2D32f> CameraPointSample;
typedef std::vector<CameraPointSample> CameraPointSamples;

/**
* compute a 3D point based on 2D image points and the corresponding camera matrices
* each camera point pair construct a ray in 3D space, we want to find the intersection of all rays
* @see PointCalculatorFactory
*/
typedef GenericModelCalculator<CvPoint3D64f, CameraPointSample> PointCalculator;
typedef GenericErrorCaclculator<CvPoint3D64f, CameraPointSample> PointErrorCalculator;

class PointGeometricErrorCalculator : public PointErrorCalculator {
public:
	double compute(const CvPoint3D64f& p, const CameraPointSample& sample) const;
};

typedef ModelEvaluationResults<CameraPointSample> PointEvaluationResults;
typedef ModelEvaluator<CvPoint3D64f, CameraPointSample, PointGeometricErrorCalculator> PointGeometricErrorCalculatorEvaluator;

}

#endif // FVISION_POINT_CALCULATOR_H_
