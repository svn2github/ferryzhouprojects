#pragma once

#include <vector>

#include "highgui.h"
#include "cv.h"

#include "Homography.h"

using namespace std;

namespace ferry {
	namespace cv_geometry {

class HomographyCalculator
{
public:
	virtual Homography compute(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s) = 0;
};

	}
}