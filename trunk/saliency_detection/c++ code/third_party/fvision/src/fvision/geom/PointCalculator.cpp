#include "PointCalculator.h"

#include <fvision/utils/geom.h>
#include "singleviewgeom.h"

namespace fvision {

double PointGeometricErrorCalculator::compute(const CvPoint3D64f& p, const CameraPointSample& sample) const {
	CvPoint2D32f rp = projectWithP(sample.first.getMat(), p);
	return pointDistance(rp, sample.second);
}

}