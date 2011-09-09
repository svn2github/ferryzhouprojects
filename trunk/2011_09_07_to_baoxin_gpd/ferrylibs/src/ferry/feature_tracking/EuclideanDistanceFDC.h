#pragma once

#include "FeatureDistanceCalculator.h"

namespace ferry {
	namespace feature_tracking {

class EuclideanDistanceFDC : public FeatureDistanceCalculator
{
public:
	float compute(IplImage* im1, IplImage* im2, const CvPoint& p1, const CvPoint& p2) {
		int dx = p1.x - p2.x;
		int dy = p1.y - p2.y;
		return dx * dx + dy * dy;
	}
};

	}
}