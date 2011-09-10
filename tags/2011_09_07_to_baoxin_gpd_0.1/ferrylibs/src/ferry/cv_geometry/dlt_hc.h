#pragma once

#include <assert.h>

#include "HomographyCalculator.h"
#include "CvMatUtil.h"

using namespace ferry::cv_mat;

namespace ferry {
	namespace cv_geometry {

class DLT_HC : public HomographyCalculator
{
public:
	//<<multiple view geometry>>(2nd) p89
	Homography compute(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s) {
		assert(x1s.size() >= 4 && x1s.size() == x2s.size()); 
		
		double* a = new double[x1s.size() * 2 * 9];
		for (size_t i = 0; i < x1s.size() * 2 * 9; i++) a[i] = 0;
		for (int i = 0; i < (int)x1s.size(); i++) {
			int offset = 18 * i;

			double x1 = x1s[i].x;
			double y1 = x1s[i].y;
			double w1 = 1;

			double x2 = x2s[i].x;
			double y2 = x2s[i].y;
			double w2 = 1;

			a[offset + 3] = -w2 * x1;
			a[offset + 4] = -w2 * y1;
			a[offset + 5] = -w2 * w1;

			a[offset + 6] = y2 * x1;
			a[offset + 7] = y2 * y1;
			a[offset + 8] = y2 * w1;

			a[offset + 9 ] = w2 * x1;
			a[offset + 10] = w2 * y1;
			a[offset + 11] = w2 * w1;

			a[offset + 15] = -x2 * x1;
			a[offset + 16] = -x2 * y1;
			a[offset + 17] = -x2 * w1;

		}

		CvMat A = cvMat(2 * (int)x1s.size(), 9, CV_64FC1, a);
		//cout<<CvMatUtil::matString(&A)<<endl;
		CvMat* na = CvMatUtil::null(&A);
		
		CvMat* H = cvCreateMat(3, 3, CV_64FC1);
		for (int i = 0; i < 9; i++) {
			H->data.db[i] = na->data.db[i];
		}
		cvReleaseMat(&na);
		return Homography(H);
	}
};

	}
}