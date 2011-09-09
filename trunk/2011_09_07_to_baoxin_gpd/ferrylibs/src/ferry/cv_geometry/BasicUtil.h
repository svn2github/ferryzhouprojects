#pragma once

#include <iostream>
#include <vector>

#include <cassert>

#include "cv.h"

#include "CvMatUtil.h"

using namespace std;
using namespace ferry::cv_mat;

namespace ferry {
	namespace cv_geometry {

		//pt: 3 x 1
		//line: 3 x 1
		double point2LineDistance(const CvMat* pt, const CvMat* line);

		vector<CvPoint> cvPointsFrom32fs(const vector<CvPoint2D32f>& pts);
		vector<CvPoint2D32f> cvPointsTo32fs(const vector<CvPoint>& pts);

		double pointDistance(const CvPoint2D32f& p1, const CvPoint2D32f& p2);

		double pointDistance(const CvPoint& p1, const CvPoint& p2);

		CvPoint3D32f scalePoint3D(CvPoint3D32f p3d, double v);
	}
}