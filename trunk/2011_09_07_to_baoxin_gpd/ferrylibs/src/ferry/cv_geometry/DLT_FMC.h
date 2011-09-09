#pragma once

#include <assert.h>

#include "FundamentalMatrixCalculator.h"
#include "CvMatUtil.h"

using namespace ferry::cv_mat;

namespace ferry {
	namespace cv_geometry {

class DLT_FMC : public FundamentalMatrixCalculator
{
public:
	FundamentalMatrix compute(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s) {
		double x1, x2, y1, y2;
		int s = (int)x1s.size();
		CvMat* A = cvCreateMat(s, 9, CV_64FC1);
		cvZero(A);
		for (int i = 0; i < s; i++) {
			x1 = x1s[i].x;
			y1 = x1s[i].y;
			x2 = x2s[i].x;
			y2 = x2s[i].y;
			cvmSet(A, i, 0, x2 * x1);
			cvmSet(A, i, 1, x2 * y1);
			cvmSet(A, i, 2, x2);
			cvmSet(A, i, 3, y2 * x1);
			cvmSet(A, i, 4, y2 * y1);
			cvmSet(A, i, 5, y2);
			cvmSet(A, i, 6, x1);
			cvmSet(A, i, 7, y1);
			cvmSet(A, i, 8, 1);
		}

		CvMat* na = CvMatUtil::null(A);
		
		CvMat* F = cvCreateMat(3, 3, CV_64FC1);
		for (int i = 0; i < 9; i++) {
			F->data.db[i] = na->data.db[i];
		}

		int n = MIN(F->rows, F->cols);
		CvMat* u = cvCreateMat(F->rows, F->rows, F->type);
		CvMat* vt = cvCreateMat(F->cols, F->cols, F->type);
		CvMat* w = cvCreateMat(F->rows, F->cols, F->type);
		
		cvSVD(F, w, u, vt, CV_SVD_V_T);

		cvmSet(w, 2, 2, 0);
		CvMat* F2 = CvMatUtil::matMul(u,w, vt);
		
		cvReleaseMat(&na);
		cvReleaseMat(&u);
		cvReleaseMat(&w);
		cvReleaseMat(&vt);

		return FundamentalMatrix(F2);
	}
};

	}
}