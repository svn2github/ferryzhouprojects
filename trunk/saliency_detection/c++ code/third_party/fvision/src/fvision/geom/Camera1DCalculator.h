#pragma once

#include "Camera1D.h"

#include <fvision/algorithm/ransac.h>
#include <fvision/utils/model_evaluation_template.h>

namespace fvision {

typedef std::pair<CvPoint2D32f, double> Projection1DSample;
typedef std::vector<Projection1DSample> Projection1DSamples;

/**
* compute camera based on 3D to 2D projection
* @see CameraCalculatorFactory
*/
typedef GenericModelCalculator<Camera1D, Projection1DSample> Camera1DCalculator;

class Camera1DErrorCalculator : public GenericErrorCaclculator<Camera1D, Projection1DSample> {
public:
	double compute(const Camera1D& cam, const Projection1DSample& sample) const;
};

typedef ModelEvaluationResults<Projection1DSample> Camera1DEvaluationResults;
typedef ModelEvaluator<Camera1D, Projection1DSample, Camera1DErrorCalculator> Camera1DErrorEvaluator;

}