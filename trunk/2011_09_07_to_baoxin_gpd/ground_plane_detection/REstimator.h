#pragma once

#include "cv.h"

#include <ferry/cv_geometry/DLT_HC.h>

using namespace ferry::cv_geometry;

class REstimator
{
public:
	REstimator(void) {}
	~REstimator(void) {}

public:
	CvMat* compute(const vector<CvPoint2D32f>& gxs, const vector<CvPoint2D32f>& ixs, CvMat* K) {
		DLT_HC hc;
		Homography H = hc.compute(gxs, ixs);
		return simpleEstimateR(H.getMat(), K);
	}

public:
	//H = K[r1, r3, t]
	//r2 = r1 x r3
	static CvMat* simpleEstimateR(CvMat* H, CvMat* K) {
		CvMat* invKH = matMul(inv(K), H);
		if (cvmGet(invKH, 0, 0) > 0) {
			cvScale(invKH, invKH, -1);
		}
		CvMat* r1 = getCol(invKH, 0);
		normalizeVector(r1);
		CvMat* r3 = getCol(invKH, 1);
		normalizeVector(r3);
		CvMat* r2 = cvCreateMat(3, 1, CV_64FC1);
		cvCrossProduct(r1, r3, r2);
		CvMat* R = cvCreateMat(3, 3, CV_64FC1);
		setCol(R, r1, 0);
		setCol(R, r2, 1);
		setCol(R, r3, 2);
		return R;
	}

};
