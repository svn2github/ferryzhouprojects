#include "Camera1DCalculator.h"

namespace fvision {

double Camera1DErrorCalculator::compute(const Camera1D& cam, const Projection1DSample& sample) const {
	double reprojection = cam.project(sample.first);
	return abs(reprojection - sample.second);	
}

}