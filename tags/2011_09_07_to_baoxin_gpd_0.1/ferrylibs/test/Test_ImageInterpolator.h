#pragma once

#include <iostream>
#include <stdio.h>

#include "highgui.h"

#include <ferry/cv_geometry/ImageInterpolator.h>
#include "Test_MSVideo.h"
#include <ferry/cv_geometry/TwoViewGeometryUtil.h>

using namespace std;
using namespace ferry::cv_geometry;

namespace ferry {
	namespace cv_geometry {
		namespace test {

class Test_ImageInterpolator
{
public:
	void test() {
		Test_MSVideo tmsv;
		tmsv.readCalibrationFile("data/MS_Video/dancing/calibParams-breakdancers.txt");
		
		Camera* pcam1 = tmsv.cams[3];
		Camera* pcam2 = tmsv.cams[5];

		char* color_left_file = "data/MS_Video/dancing/color-cam3-f000.bmp";
		char* color_right_file = "data/MS_Video/dancing/color-cam5-f000.bmp";
		char* depth_left_file = "data/MS_Video/dancing/depth-cam3-f000.bmp";
		char* depth_right_file = "data/MS_Video/dancing/depth-cam5-f000.bmp";
		char* dst_file = "data/MS_Video/temp/interpolate-cam4-cam5-fix-f000.bmp";

		IplImage* colorLeftImage = cvLoadImage(color_left_file, 1);
		IplImage* colorRightImage = cvLoadImage(color_right_file, 1);
		IplImage* depthLeftImage = cvLoadImage(depth_left_file, 0);
		IplImage* depthRightImage = cvLoadImage(depth_right_file, 0);

		ImageInterpolator ii(colorLeftImage, colorRightImage, depthLeftImage, depthRightImage, pcam1, pcam2);

		Camera cam(tmsv.cams[4]->getK(), getNP(-1.0));

		IplImage* dst_im = ii.interpolate(&cam);

		cvSaveImage(dst_file, dst_im);
	}

	CvMat* getNP(double x) {
		CvMat* NP = cvCreateMat(3, 4, CV_64FC1);
		cvZero(NP);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				if (i == j) cvmSet(NP, i, j, 1.0);
			}
		}
		cvmSet(NP, 0, 3, x);
		return NP;
	}

};

		}
	}
}