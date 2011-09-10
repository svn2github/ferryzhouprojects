#pragma once

#include <vector>

#include "highgui.h"
#include "cv.h"

#include "FundamentalMatrix.h"

using namespace std;

namespace ferry {
	namespace cv_geometry {

class FundamentalMatrixCalculator
{
public:
	virtual FundamentalMatrix compute(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s) = 0;
};

	}
}