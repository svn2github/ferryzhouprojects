#pragma once

#include <iostream>
#include <fstream>
#include "highgui.h"

#include <ferry/cv_geometry/IOUtil.h>

using namespace std;
using namespace ferry::cv_geometry;
using namespace ferry::cv_geometry::io_util;

namespace ferry {
	namespace cv_geometry {
		namespace test {

class Test_GeneratePatch
{
public:
	void test() {
		char* infile = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/urban_map/scene_1/small/im_db/full_t0_k0.jpg";
		char* outfile = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/urban_map/scene_1/small/im_db/seg_t0_k0_s7.jpg";
		char* polygonfile = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/urban_map/scene_1/small/im_db/seg_t0_k0_s7.txt";
		
		gen(infile, outfile, polygonfile);
	}

	void gen(const char* infile, const char* outfile, const char* polygonfile) {

		IplImage* src = cvLoadImage(infile, 1);
		IplImage* dst = cvCreateImage(cvGetSize(src), 8, 3);

		vector<CvPoint> xs;
		ifstream ifs(polygonfile);
		ifs>>xs;
		ifs.close();

		CvPoint* polygon = new CvPoint[xs.size()];
		for (int i = 0; i < xs.size(); i++) {
			polygon[i] = xs[i];
		}

		cout<<xs<<endl;

		IplImage* pim = cvCreateImage(cvGetSize(src), 8, 1);
		cvZero(pim);
		int size[] = {xs.size()};
		cvFillPoly(pim, &polygon, size, 1, CV_RGB(255, 255, 255));

		cvNamedWindow("src", 1);
		cvShowImage("src", src);

		cvNamedWindow("poly", 1);
		cvShowImage("poly", pim);

		cvCopy(src, dst, pim);

		cvNamedWindow("dst", 1);
		cvShowImage("dst", dst);

		cvSaveImage(outfile, dst);
	}

};

		}
	}
}