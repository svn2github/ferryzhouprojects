#pragma once

#include <vector>

#include "highgui.h"
#include "cv.h"

using namespace std;

namespace ferry {
	namespace feature_tracking {

class CornerDetector
{
public:
	virtual void compute(IplImage* src, vector<CvPoint>& corners) = 0;
};
	}
}
