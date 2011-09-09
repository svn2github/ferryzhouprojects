#pragma once

#include <iostream>
#include <stdio.h>
#include <fstream>

#include "highgui.h"

#include <ferry/cv_geometry/TwoViewGeometryUtil.h>
#include <ferry/cv_geometry/DrawingUtil.h>
#include <ferry/cv_geometry/IOUtil.h>

using namespace std;
using namespace ferry::cv_geometry;
using namespace ferry::cv_geometry::io_util;

namespace ferry {
	namespace cv_geometry {
		namespace test {

class Test_ImageRotation
{
public:
	void test() {

		double len = 500;
		CvMat* K = getIdentity();
		cvmSet(K, 0, 0, len);
		cvmSet(K, 1, 1, len);
		cvmSet(K, 0, 2, len / 2);
		cvmSet(K, 1, 2, len / 2);

		CvMat* K2 = getIdentity();
		cvmSet(K2, 0, 0, len/1.5);
		cvmSet(K2, 1, 1, len/1.5);
		cvmSet(K2, 0, 2, len / 2);
		cvmSet(K2, 1, 2, len / 2);
		double m_Theta = 20;

		//CvMat* RH = getHWithRotate(K, m_Theta * 3.1416 / 180);

		double tanTheta1 = tan(m_Theta * 3.1416 / 180);
		double rh[] = {1, 0, tanTheta1,  0, 1, 0,  -tanTheta1, 0, 1};
		CvMat RH = cvMat(3, 3, CV_64FC1, rh);

		double tanTheta = tan(0 * 3.1416 / 180);
		double mh[] = {1, tanTheta, 0, -tanTheta, 1, 0, 0,  0, 1};
		CvMat MH = cvMat(3, 3, CV_64FC1, mh);

		//IplImage* im1 = cvLoadImage("temp/view.bmp", 1);
		//IplImage* im3 = cvCreateImage(cvSize(500, 500), 8, 3);
		IplImage* im1 = cvLoadImage("temp/im3.bmp", 1);
		IplImage* im3 = cvCreateImage(cvSize(800, 700), 8, 3);
		//cvSet(im3, CV_RGB(255, 255, 255));
		//cvNamedWindow("rm1", 1);
		//cvShowImage("rm1", im3);

		CvMat* T = getIdentity();
		cvmSet(T, 0, 2, -100);
		cvmSet(T, 1, 2, 100);

		CvMat* RRH = matMul(&RH, &MH);

		CvMat* rrh = matMul(T, matMul(K, RRH, inv(K)));
		//CvMat* rrh = inv(matMul(K, RRH, inv(K)));

		//cvWarpPerspective(im1, im3, rrh);
		cvWarpPerspective(im1, im3, rrh, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, CV_RGB(255, 255, 255));

		vector<CvPoint2D32f> x1s;
		x1s.push_back(cvPoint2D32f(0, 250));
		x1s.push_back(cvPoint2D32f(500, 250));
		x1s.push_back(cvPoint2D32f(250, 0));
		x1s.push_back(cvPoint2D32f(250, 500));

		vector<CvPoint2D32f> nxs = transformPoints(rrh, x1s);
		double d1 = pointDistance(nxs[0], nxs[1]);
		double d2 = pointDistance(nxs[2], nxs[3]);
		cout<<d1<<" "<<d2<<endl;
		CvPoint2D32f l1 = cvPoint2D32f(nxs[0].x - nxs[1].x, nxs[0].y - nxs[1].y);
		CvPoint2D32f l2 = cvPoint2D32f(nxs[2].x - nxs[3].x, nxs[2].y - nxs[3].y);
		cout<<(l1.x * l2.x + l1.y * l2.y) / (sqrt(l1.x * l1.x + l1.y * l1.y) * sqrt(l2.x * l2.x + l2.y * l2.y))<<endl;

		//cvLine(im3, cvPointFrom32f(x1s[0]), cvPointFrom32f(x1s[1]), CV_RGB(0, 255, 0), 3);
		//cvLine(im3, cvPointFrom32f(x1s[2]), cvPointFrom32f(x1s[3]), CV_RGB(0, 255, 0), 3);
		//cvSaveImage("temp/im3.bmp", im3);

		cvLine(im3, cvPointFrom32f(nxs[0]), cvPointFrom32f(nxs[1]), CV_RGB(0, 255, 0), 3);
		cvLine(im3, cvPointFrom32f(nxs[2]), cvPointFrom32f(nxs[3]), CV_RGB(0, 255, 0), 3);
/*
		vector<CvPoint2D32f> x2s;
		x2s.push_back(cvPoint2D32f(0, 0));
		x2s.push_back(cvPoint2D32f(0, 500));
		x2s.push_back(cvPoint2D32f(500, 500));
		x2s.push_back(cvPoint2D32f(500, 0));
		vector<CvPoint2D32f> nx2s = transformPoints(rrh, x2s);

		cvLine(im3, cvPointFrom32f(nx2s[0]), cvPointFrom32f(nx2s[1]), CV_RGB(0, 255, 0));
		cvLine(im3, cvPointFrom32f(nx2s[1]), cvPointFrom32f(nx2s[2]), CV_RGB(0, 255, 0));
		cvLine(im3, cvPointFrom32f(nx2s[2]), cvPointFrom32f(nx2s[3]), CV_RGB(0, 255, 0));
		cvLine(im3, cvPointFrom32f(nx2s[3]), cvPointFrom32f(nx2s[0]), CV_RGB(0, 255, 0));
*/
		cvNamedWindow("rm", 1);
		cvShowImage("rm", im3);

		//cvSaveImage("temp/rec_im3.bmp", im3);
		
	}

};

		}
	}
}