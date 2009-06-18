#ifndef FVISION_CAMERA_CALCULATOR_H_
#define FVISION_CAMERA_CALCULATOR_H_

#include "Camera.h"

#include <fvision/algorithm/ransac.h>
#include <fvision/utils/model_evaluation_template.h>

namespace fvision {

typedef std::pair<CvPoint3D64f, CvPoint2D32f> ProjectionSample;
typedef std::vector<ProjectionSample> ProjectionSamples;

/**
* compute camera based on 3D to 2D projection
* @see CameraCalculatorFactory
*/
typedef GenericModelCalculator<Camera, ProjectionSample> CameraCalculator;

class CameraErrorCalculator : public GenericErrorCaclculator<Camera, ProjectionSample> {
public:
	double compute(const Camera& cam, const ProjectionSample& sample) const;
};

typedef ModelEvaluationResults<ProjectionSample> CameraEvaluationResults;
typedef ModelEvaluator<Camera, ProjectionSample, CameraErrorCalculator> CameraErrorEvaluator;

}
#endif // FVISION_CAMERA_CALCULATOR_H_