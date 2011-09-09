#pragma once

#include "FundamentalMatrixCalculator.h"
#include "CvMatUtil.h"

using namespace ferry::cv_mat;

namespace ferry {
	namespace cv_geometry {

class OpenCV_FMC : public FundamentalMatrixCalculator
{
public:
	OpenCV_FMC(int method = CV_FM_RANSAC, double param1 = 0.5, double param2 = 0.99, CvMat* status = NULL) {
		this->method = method;
		this->param1 = param1;
		this->param2 = param2;
		this->status = status;
	}

public:
	FundamentalMatrix compute(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s) {
		CvMat* F = cvCreateMat(3, 3, CV_64FC1);
		int N = (int)x1s.size();
		CvMat* points1 = cvCreateMat(N, 2, CV_64FC1);
		CvMat* points2 = cvCreateMat(N, 2, CV_64FC1);
		for (int i = 0; i < N; i++) {
			cvmSet(points1, i, 0, x1s[i].x);
			cvmSet(points1, i, 1, x1s[i].y);
			cvmSet(points2, i, 0, x2s[i].x);
			cvmSet(points2, i, 1, x2s[i].y);
		}
		cvFindFundamentalMat(points1, points2, F, method, param1, param2, status);
		cout<<"F: "<<F<<endl;
		cvReleaseMat(&points1);
		cvReleaseMat(&points2);
		return FundamentalMatrix(F);
	}

private:
	int method;
	double param1;
	double param2;
	CvMat* status;
};

	}
}