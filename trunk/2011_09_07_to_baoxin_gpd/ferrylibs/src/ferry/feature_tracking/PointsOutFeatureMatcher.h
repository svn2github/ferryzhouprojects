#pragma once

#include <vector>

#include "highgui.h"
#include "cv.h"

#include "FeatureMatcher.h"

using namespace std;

namespace ferry {
	namespace feature_tracking {

class PointsOutFeatureMatcher
{
public:
	//input corners
	//output point correspondences. x1s.size() == x2s.size()
	virtual void compute(IplImage* im1, IplImage* im2, const vector<CvPoint>& corners1, const vector<CvPoint>& corners2, vector<CvPoint>& x1s, vector<CvPoint>& x2s) = 0;
};

	}
}