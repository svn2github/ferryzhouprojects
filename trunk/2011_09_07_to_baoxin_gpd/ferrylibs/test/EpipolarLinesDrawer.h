#pragma once

#include <iostream>
#include <fstream>

#include <highgui.h>

#include <ferry/cv_geometry/CvMatUtil.h>
#include <ferry/cv_geometry/IOUtil.h>

using namespace std;
using namespace ferry::cv_mat;
using namespace ferry::cv_geometry::io_util;

namespace ferry {
	namespace cv_geometry {
		namespace test {

class EpipolarLinesDrawer
{
public:
	void test() {
		char* file1 = "data/MS_Video/dancing/3_jpg.jpg";
		char* file2 = "data/MS_Video/dancing/4_jpg.jpg";
		IplImage* im1 = cvLoadImage(file1, 1);
		IplImage* im2 = cvLoadImage(file2, 1);
		
		CvMat* F;
		ifstream ifs("temp/F.txt");
		ifs>>(&F);
		ifs.close();

		cout<<"F: "<<F<<endl;

		drawEpipoleLines(im1, im2, F);

		cvNamedWindow("im1", 1);
		cvNamedWindow("im2", 1);
		cvShowImage("im1", im1);
		cvShowImage("im2", im2);
	}

	void drawEpipoleLines(IplImage* im1, IplImage* im2, CvMat* F) {
		CvMat* e1 = CvMatUtil::null(F);

		for (int i = 1; i < 5; i++) {
			drawEpipoleLine(im1, im2, F, e1, cvPoint2D32f(0, i * 100));
		}
	}

	void drawEpipoleLine(IplImage* im1, IplImage* im2, CvMat* F, CvMat* e1, CvPoint2D32f p1) {
		CvMat* mx1 = CvMatUtil::hmatFromPoint2D(p1);
		CvMat* l1 = cvCreateMat(3, 1, CV_64FC1);
		cvCrossProduct(e1, mx1, l1);
		CvMat* l2 = CvMatUtil::matMul(F, mx1);

		drawLine(im1, l1);
		drawLine(im2, l2);
	}

	void drawLine(IplImage* im, CvMat* line) {
		double x0 = 0;
		double x1 = im->width;
		double a = line->data.db[0];
		double b = line->data.db[1];
		double c = line->data.db[2];
		double y0 = - (a * x0 + c) / b;
		double y1 = - (a * x1 + c) / b;
		cvLine(im, cvPoint((int)x0, (int)(im->height - y0 - 1)), cvPoint((int)x1, (int)(im->height - y1 - 1)), CV_RGB(255, 255, 255));
	}

	CvMat* getF() {
		double f[] = {0.000287572/100, 0.0128726/100, 0.00726273*100,
			          -0.0080811/100, 0.000223544/100, 0.698268*100,
					  -0.021264*100, -0.712918*100, 0.058599*100};
		CvMat* F = cvCreateMat(3, 3, CV_64FC1);
		cvSetData(F, f, F->step);
		return F;
	}
};

		}
	}
}