#pragma once

#include "cv.h"

#include "CvMatUtil.h"
#include "IOUtil.h"
//#include "TwoViewGeometryUtil.h"
//#include "BasicUtil.h"

//using namespace std;
using namespace ferry::cv_mat;
using namespace ferry::cv_geometry::io_util;

namespace ferry {
	namespace cv_geometry {

class CrossOpticalAxes_PBIR
{
public:
	void compute(CvMat* K, float tanTheta, CvMat** H1, CvMat** H2) {
		compute(K, tanTheta, H1);
		compute(K, -tanTheta, H2);
	}

public:
	static void compute(CvMat* K, float tanTheta, CvMat** H) {

		//float theta = 5 * 3.1415 / 180;
		double mh[] = {1, 0, tanTheta,  0, 1, 0,  -tanTheta, 0, 1};
		CvMat MH = cvMat(3, 3, CV_64FC1, mh);

		cout<<"MH: "<<&MH<<endl;
		*H = matMul(K, &MH, inv(K));

	}
};

	}
}
