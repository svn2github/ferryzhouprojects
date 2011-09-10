#pragma once

#include <iostream>
#include <fstream>

#include "highgui.h"

#include <ferry/cv_geometry/ImageRectifier.h>
#include <ferry/cv_geometry/TwoViewGeometryUtil.h>
#include <ferry/cv_geometry/DrawingUtil.h>
#include <ferry/cv_geometry/IOUtil.h>

using namespace std;
using namespace ferry::cv_geometry;
using namespace ferry::cv_geometry::io_util;

namespace ferry {
	namespace cv_geometry {
		namespace test {

class Test_RectificationGroupPoints
{
public:
	void test() {
		//testK2();
		testGroupPoints();
	}

	void testGroupPoints() {
		IplImage* im1;
		IplImage* im2;

		PointCorrespondences pc;

		char* dir = "data/olympus/scen-15";

		char pcfile[100];
		sprintf(pcfile, "%s/ixs.txt", dir);


		ifstream ifs1(pcfile);
		ifs1>>pc;
		ifs1.close();

		char im1_file[200];
		sprintf(im1_file, "%s/1_small.jpg", dir);
		char im2_file[200];
		sprintf(im2_file, "%s/3_small.jpg", dir);

		im1 = cvLoadImage(im1_file, 1);
		im2 = cvLoadImage(im2_file, 1);

		IplImage* im4 = ::create_correspondences_image(im1, cvPointsFrom32fs(pc.x1s), cvPointsFrom32fs(pc.x2s));
		char total[100];
		sprintf(total, "%s/cluster_total.jpg", dir);
		cvSaveImage(total, im4);

		char file[200];
		char jpgfile[200];
		for (int j = 0; j < 25; j++) {
			sprintf(file, "%s/cluster%d.txt", dir, j);
			sprintf(jpgfile, "%s/cluster%d.jpg", dir, j);

			vector<int> cluster;
			ifstream ifs(file);
			int n;
			ifs>>n;
			for (int i = 0; i < n; i++) {
				int t;
				ifs>>t;
				cluster.push_back(t);
			}
			ifs.close();

			vector<CvPoint> cx1s, cx2s;
			for (int i = 0; i < cluster.size(); i++) {
				cx1s.push_back(cvPointFrom32f(pc.x1s[cluster[i]]));
				cx2s.push_back(cvPointFrom32f(pc.x2s[cluster[i]]));
			}

			IplImage* im3 = ::create_correspondences_image(im1, cx1s, cx2s);
			//IplImage* im3 = ::create_correspondences_image(im1, cvPointsFrom32fs(pc.x1s), cvPointsFrom32fs(pc.x2s));
			cvSaveImage(jpgfile, im3);

			cvNamedWindow(file, 1);
			cvShowImage(file, im3);
		}
	}

	void testK() {

		CvMat* H1;
		CvMat* H2;

		ifstream ifs1("data/olympus/30/eK1.txt");
		ifs1>>&H1;
		ifs1.close();

		ifstream ifs2("data/olympus/30/eK2.txt");
		ifs2>>&H2;
		ifs2.close();

		cout<<matMul(H1, inv(H2))<<endl;

	}

	void testK2() {
		double w = 1024;
		double h = 768;
		CvSize im_size = cvSize(w, h);

		double true_theta = 90;
		double theta = 60;
		double true_f = (double)(im_size.width / 2 / tan(true_theta*3.1416/180/2));
		double p = 5;
		double e2x = 2 * p * true_f;
		double e2y = 0.2 * e2x;
		CvMat* e2 = cvCreateMat(3, 1, CV_64FC1);
		cvmSet(e2, 0, 0, e2x);
		cvmSet(e2, 1, 0, e2y);
		cvmSet(e2, 2, 0, 1);

		CvMat* H1 = estimateH2(theta, e2, im_size);
		CvMat* H2 = estimateH2(true_theta, e2, im_size);

		cout<<"H1: "<<H1<<endl;

		cout<<matMul(H1, inv(H2))<<endl;
	}

	CvMat* estimateH2(double alpha, CvMat* e2, CvSize im_size) {

		double f = (double)(im_size.width / 2 / tan(alpha*3.1416/180/2));
		cout<<"2tan: "<<2 * tan(alpha*3.1416/180/2)<<endl;
		
		double px = (double)im_size.width / 2;
		double py = (double)im_size.height / 2;
		//double k2[] = {f, 0, px, 0, f, py, 0, 0, 1};
		double k2[] = {f, 0, px, 0, -f, py, 0, 0, 1};
		CvMat  sK2 = cvMat(3, 3, CV_64FC1, k2);
		CvMat* K2 = &sK2;
		CvMat* K0 = cloneMat(&sK2);
		CvMat* inv_K2 = cvCreateMat(3, 3, CV_64FC1);
		cvInvert(K2, inv_K2);
		
		//CvMat* e2 = leftNull(F);
		CvMat* r = matMul(inv_K2, e2);
		normalizeVector(r);
		if (cvmGet(r, 0, 0) < 0) {
			cvScale(r, r, -1.0);
		}
		CvMat* s2 = matFromPoint3D(cvPoint3D32f(0, 1, 0));
		CvMat* t = cvCreateMat(3, 1, CV_64FC1);
		cvCrossProduct(r, s2, t);
		normalizeVector(t);
		CvMat* s = cvCreateMat(3, 1, CV_64FC1);
		cvCrossProduct(t, r, s);

		CvMat* R0t = cvCreateMat(3, 3, CV_64FC1);
		setSubRect(R0t, r, cvRect(0, 0, 1, 3));
		setSubRect(R0t, s, cvRect(1, 0, 1, 3));
		setSubRect(R0t, t, cvRect(2, 0, 1, 3));
		CvMat* R0 = cvCreateMat(3, 3, CV_64FC1);
		cvT(R0t, R0);

		//==DEBUG
		cout<<"R0: "<<R0<<endl;
		//==

		CvMat* H2 = matMul(K0, R0, inv_K2);

		return H2;
	}

};

		}
	}
}