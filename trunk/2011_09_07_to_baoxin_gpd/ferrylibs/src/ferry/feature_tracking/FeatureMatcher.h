#pragma once

#include <vector>

#include "highgui.h"
#include "cv.h"

using namespace std;

#define MAX_FEATURE_NUM 400

namespace ferry {
	namespace feature_tracking {

class FeatureMatcher
{
public:
	//input corners
	//output point correspondences indices
	virtual void compute(IplImage* im1, IplImage* im2, const vector<CvPoint>& corners1, const vector<CvPoint>& corners2, int* correspondences1, int* correspondences2) = 0;
};

	}
}