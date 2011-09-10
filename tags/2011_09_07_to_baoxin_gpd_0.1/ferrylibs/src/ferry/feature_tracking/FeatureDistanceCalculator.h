#pragma once

#include "highgui.h"
#include "cv.h"

#define MAX_DISTANCE 1000000.0 

namespace ferry {
	namespace feature_tracking {

class FeatureDistanceCalculator
{
public:
	//compute distance. not necesarily based on euclidean distance. may be dissimilarity.
	virtual float compute(IplImage* im1, IplImage* im2, const CvPoint& p1, const CvPoint& p2) = 0;
};

	}
}