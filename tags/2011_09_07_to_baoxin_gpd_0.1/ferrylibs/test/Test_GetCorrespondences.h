#pragma once

#include "Test_FeatureMatcher.h"
#include "FeatureTracking.h"

class Correspondences {
public:
	Correspondences(IplImage* im1,
		IplImage* im2,
		const vector<CvPoint>& x1s,
		const vector<CvPoint>& x2s) : im1(im1), im2(im2), x1s(x1s), x2s(x2s) {
		}

public:
	IplImage* im1;
	IplImage* im2;
	//IplImage* g_im1;
	//IplImage* g_im2;

	vector<CvPoint> x1s;
	vector<CvPoint> x2s;
};

class Test_GetCorrespondences
{
public:
	Correspondences* getCorrespondences() {
		return new Correspondences(im1, im2, x1s, x2s);
	}

public:
	void loadFeatureMatcher() {
		Test_FeatureMatcher* tf = new Test_FeatureMatcher();
		tf->test();
		this->im1 = tf->im1;
		this->im2 = tf->im2;
		this->x1s = tf->x1s;
		this->x2s = tf->x2s;
	}

	void loadComputedData() {
		char* x1sfile = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/temp/ix1s.txt";
		char* x2sfile = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/temp/ix2s.txt";
		IOUtil::readPoints(x1sfile, x1s);
		IOUtil::readPoints(x2sfile, x2s);
		
		char* im1file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/olympus/1_small.jpg";
		char* im2file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/olympus/3_small.jpg";
		im1 = cvLoadImage(im1file, 1);
		im2 = cvLoadImage(im2file, 1);
	}

	void loadSimulatedData() {
		vector<Matrix2D> Xs;
		for (int i = 3; i < 5; i++) {
			for (int j = -2; j < 3; j++) {
				Xs.push_back(Matrix2DUtil::initPoint3D(i * 2, j * 2, 0));
			}
		}
		for (i = -1; i < 2; i++) {
			for (int j = 1; j < 4; j++) {
				Xs.push_back(Matrix2DUtil::initPoint3D(3, i * 2, j));
			}
		}

		Camera cam1(1, Matrix2DUtil::initPoint3D(14, 0, 4), Matrix2DUtil::initPoint3D(-1, 0, 0), Matrix2DUtil::initPoint3D(0, 0, 1));
		Camera cam2(1, Matrix2DUtil::initPoint3D(13.5, 1.0, 4), Matrix2DUtil::initPoint3D(-1, 0, 0), Matrix2DUtil::initPoint3D(0, 0, 1));

		//Camera cam1(1, Matrix2DUtil::initPoint3D(11, 0, 10), Matrix2DUtil::initPoint3D(-1, 0, -1.5), Matrix2DUtil::initPoint3D(0, 0, 1));
		//Camera cam2(1, Matrix2DUtil::initPoint3D(10.5, 1, 10), Matrix2DUtil::initPoint3D(-1, 0, -1.5), Matrix2DUtil::initPoint3D(0, 0, 1));

		TwoViewSimulation tvs;
		tvs.setModel(Xs);
		tvs.setCameras(&cam1, &cam2);
		tvs.calcImagePoints();
		tvs.addSomeNoise(0.001);

		im1 = cvCreateImage(cvSize(400, 400), 8, 3);
		im2 = cvCreateImage(cvSize(400, 400), 8, 3);
		CvPointMatrix2DConvertInterface* p_pmci = new WindowPMC(im1->width, im1->height);

		p_pmci->matrix2Ds2CvPoints(tvs.getx1s(), x1s);
		p_pmci->matrix2Ds2CvPoints(tvs.getx2s(), x2s);
	}

public:
	IplImage* im1;
	IplImage* im2;
	//IplImage* g_im1;
	//IplImage* g_im2;

	vector<CvPoint> x1s;
	vector<CvPoint> x2s;

};
