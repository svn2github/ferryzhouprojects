#pragma once

#include <vector>

#include "cv.h"
#include "highgui.h"

using namespace std;

namespace ferry {
	namespace cv_geometry {

IplImage* concatenateImage(const IplImage* src1, const IplImage* src2);

void addWeightedWithBlack(const IplImage* src1, double alpha, const IplImage* src2, double beta, IplImage* dst);

void addOverlapWithBlack(const IplImage* src, IplImage* dst);

	}
}