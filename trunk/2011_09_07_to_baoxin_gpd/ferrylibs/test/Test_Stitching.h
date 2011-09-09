#pragma once

#include <iostream>
#include <fstream>

#include "cv.h"
#include "highgui.h"

#include <ferry/cv_geometry/TwoViewGeometryUtil.h>
#include <ferry/cv_geometry/ImUtil.h>
#include <ferry/cv_geometry/DrawingUtil.h>

using namespace std;
using namespace ferry::cv_geometry;

namespace ferry {
	namespace cv_geometry {
		namespace test {

class Test_Stitching
{
public:
	void test() {
		twoview();
		//stitch2();
	}

	void twoview() {
/*		char* im1file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/2/1.jpg";
		char* im2file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/2/2.jpg";
		char* Hfile = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/2/H.txt";
		char* pc1file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/2/ixs.txt";
		char* pc2file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/2/xs.txt";
		char* cos1file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/2/cos1.bmp";
		char* cos2file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/2/cos2.bmp";
		char* unionfile = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/2/1_2.bmp";
*/
/*
		char* im1file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/3/1.jpg";
		char* im2file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/3/3.jpg";
		char* Hfile = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/3/H13.txt";
		char* pc1file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/3/ixs13.txt";
		char* pc2file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/3/xs13.txt";
		char* cos1file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/3/cos1.bmp";
		char* cos2file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/3/cos2.bmp";
		char* unionfile = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/3/1_3.bmp";
*/
/*
		char* im1file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/4/3.bmp";
		char* im2file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/4/4.jpg";
		char* Hfile = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/4/H34.txt";
		char* pc1file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/4/ixs34.txt";
		char* pc2file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/4/xs34.txt";
		char* cos1file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/4/cos1.bmp";
		char* cos2file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/4/cos2.bmp";
		char* unionfile = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/4/3_4.bmp";
*/

		char* im1file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/5/2.jpg";
		char* im2file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/5/3.jpg";
		char* Hfile = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/5/H23.txt";
		char* pc1file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/5/ixs23.txt";
		char* pc2file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/5/xs23.txt";
		char* cos1file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/5/cos1.bmp";
		char* cos2file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/5/cos2.bmp";
		char* unionfile = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/5/2_3.bmp";

		IplImage* im1 = cvLoadImage(im1file, 1);
		IplImage* im2 = cvLoadImage(im2file, 1);

		CvMat* H;
		ifstream ifs(Hfile);
		ifs>>&H;
		ifs.close();
		cout<<"H: "<<H<<endl;

		PointCorrespondences pc;
		ifstream ifs1(pc1file);
		ifs1>>pc;
		ifs1.close();
		IplImage* icos = create_correspondences_image(im1, im2, cvPointsFrom32fs(pc.x1s), cvPointsFrom32fs(pc.x2s));
		cvSaveImage(cos1file, icos);

		PointCorrespondences pc2;
		ifstream ifs2(pc2file);
		ifs2>>pc2;
		ifs2.close();
		IplImage* cos2 = create_correspondences_image(im1, im2, cvPointsFrom32fs(pc2.x1s), cvPointsFrom32fs(pc2.x2s));
		cvSaveImage(cos2file, cos2);

		CvSize size = cvGetSize(im1);
		CvSize usize = cvSize(900, 600);
		IplImage* im11 = cvCreateImage(usize, 8, 3);
		cvWarpPerspective(im1, im11, getIdentity());
		IplImage* im21 = cvCreateImage(usize, 8, 3);
		cvWarpPerspective(im2, im21, inv(H));
		IplImage* im3 = cvCreateImage(usize, 8, 3);
		addWeightedWithBlack(im21, 0.5, im11, 0.5, im3);
		//cvAddWeighted(im3, 0.5, im11, 0.5, 0, im3);

		cvSaveImage(unionfile, im3);

		cvNamedWindow("w", 1);
		cvShowImage("w", im3);
	}

	CvMat* loadH(const char* filename) {
		CvMat* H;
		ifstream ifs(filename);
		ifs>>&H;
		ifs.close();
		return H;
	}

	char* getHFile(char* buffer, const char* dir, int i1, int i2) {
		sprintf(buffer, "%s/H%d%d.txt", dir, i1, i2);
		return buffer;
	}

	void stitch() {

		char* dir = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/3";
		char imfile[6][200];
		IplImage* im[6];

		for (int i = 0; i < 6; i++) {
			sprintf(imfile[i], "%s/%d.jpg", dir, i+1);
			im[i] = cvLoadImage(imfile[i], 1);
		}

		char buffer[200];
		CvMat* H13 = loadH(getHFile(buffer, dir, 1, 3));
		CvMat* H23 = loadH(getHFile(buffer, dir, 2, 3));
		CvMat* H25 = loadH(getHFile(buffer, dir, 2, 5));
		CvMat* H45 = loadH(getHFile(buffer, dir, 4, 5));
		CvMat* H46 = loadH(getHFile(buffer, dir, 4, 6));

		CvSize size = cvGetSize(im[0]);
		IplImage* imt = cvCreateImage(cvSize(1400, 1000), 8, 3);
		IplImage* imt1 = cvCreateImage(cvSize(1400, 1000), 8, 3);
		IplImage* imb = cvCreateImage(cvSize(1400, 1000), 8, 3);
		cvWarpPerspective(im[0], imt, getIdentity());
		cvCopy(imt, imt1);

		CvMat* H31 = inv(H13);
		cvWarpPerspective(im[2], imb, H31);
		addWeightedWithBlack(imt1, 0.5, imb, 0.5, imt);
		cvCopy(imt, imt1);

		CvMat* H21 = matMul(H31, H23);
		cvWarpPerspective(im[1], imb, H21);
		addWeightedWithBlack(imt1, 0.5, imb, 0.5, imt);
		cvCopy(imt, imt1);

		CvMat* H51 = matMul(H21, inv(H25));
		cvWarpPerspective(im[4], imb, H51);
		addWeightedWithBlack(imt1, 0.5, imb, 0.5, imt);
		cvCopy(imt, imt1);

		CvMat* H41 = matMul(H51, H45);
		cvWarpPerspective(im[3], imb, H41);
		addWeightedWithBlack(imt1, 0.5, imb, 0.5, imt);
		cvCopy(imt, imt1);

		CvMat* H61 = matMul(H41, inv(H46));
		cvWarpPerspective(im[5], imb, H61);
		addWeightedWithBlack(imt1, 0.5, imb, 0.5, imt);
		cvCopy(imt, imt1);

		cvSaveImage("full.bmp", imt);

		cvNamedWindow("w", 1);
		cvShowImage("w", imt);

		for (int i = 0; i < 6; i++) {
			cvReleaseImage(&(im[i]));
		}
		cvReleaseImage(&imt);
		cvReleaseImage(&imt1);
		cvReleaseImage(&imb);
	}

	void stitch2() {

		char* dir = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/map/5";
		char imfile[6][200];
		IplImage* im[6];

		for (int i = 0; i < 6; i++) {
			sprintf(imfile[i], "%s/%d.jpg", dir, i+1);
			im[i] = cvLoadImage(imfile[i], 1);
		}

		char buffer[200];
		CvMat* H12 = loadH(getHFile(buffer, dir, 1, 2));
		CvMat* H23 = loadH(getHFile(buffer, dir, 2, 3));
		CvMat* H34 = loadH(getHFile(buffer, dir, 3, 4));
		CvMat* H45 = loadH(getHFile(buffer, dir, 4, 5));
		CvMat* H56 = loadH(getHFile(buffer, dir, 5, 6));

		CvSize size = cvGetSize(im[0]);
		IplImage* imt = cvCreateImage(cvSize(1400, 1000), 8, 3);
		IplImage* imt1 = cvCreateImage(cvSize(1400, 1000), 8, 3);
		IplImage* imb = cvCreateImage(cvSize(1400, 1000), 8, 3);
		cvWarpPerspective(im[0], imt, getIdentity());
		cvCopy(imt, imt1);

		CvMat* H21 = inv(H12);
		cvWarpPerspective(im[1], imb, H21);
		addWeightedWithBlack(imt1, 0.5, imb, 0.5, imt);
		cvCopy(imt, imt1);

		CvMat* H31 = matMul(H21, inv(H23));
		cvWarpPerspective(im[2], imb, H31);
		addWeightedWithBlack(imt1, 0.5, imb, 0.5, imt);
		cvCopy(imt, imt1);

		CvMat* H41 = matMul(H31, inv(H34));
		cvWarpPerspective(im[3], imb, H41);
		addWeightedWithBlack(imt1, 0.5, imb, 0.5, imt);
		cvCopy(imt, imt1);

		CvMat* H51 = matMul(H41, inv(H45));
		cvWarpPerspective(im[4], imb, H51);
		addWeightedWithBlack(imt1, 0.5, imb, 0.5, imt);
		cvCopy(imt, imt1);

		CvMat* H61 = matMul(H51, inv(H56));
		cvWarpPerspective(im[5], imb, H61);
		addWeightedWithBlack(imt1, 0.5, imb, 0.5, imt);
		cvCopy(imt, imt1);

		char full[200];
		sprintf(full, "%s/full.bmp", dir);

		cvSaveImage(full, imt);

		cvNamedWindow("w", 1);
		cvShowImage("w", imt);

		for (int i = 0; i < 6; i++) {
			cvReleaseImage(&(im[i]));
		}
		cvReleaseImage(&imt);
		cvReleaseImage(&imt1);
		cvReleaseImage(&imb);
	}
};

		}
	}
}