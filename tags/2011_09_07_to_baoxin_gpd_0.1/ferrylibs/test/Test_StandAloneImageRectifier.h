#pragma once

#pragma once

#include <iostream>
#include <stdio.h>
#include <fstream>

#include "highgui.h"

#include <ferry/cv_geometry/ImageRectifier.h>
#include <ferry/cv_geometry/TwoViewGeometryUtil.h>
#include <ferry/cv_geometry/DrawingUtil.h>
#include <ferry/cv_geometry/IOUtil.h>
#include <ferry/cv_geometry/FundamentalMatrix.h>
#include <ferry/cv_geometry/RANSAC_FMC.h>
#include <ferry/cv_geometry/Normalized_FMC.h>

#include <ferry/feature_tracking/TwoViewFeatureTracker.h>

using namespace std;
using namespace ferry::cv_geometry;
using namespace ferry::cv_geometry::io_util;
using namespace ferry::feature_tracking;

namespace ferry {
	namespace cv_geometry {
		namespace test {

/**
 * Input: an image pair
 * Input: control parameters
 * Output: rectified images
 * Output: point correspondences, Fundamental matrix, Rectify matrices
 */
class Test_StandAloneImageRectifier
{

private:
	string trackerParamsFilename;
	string im1Filename;
	string im2Filename;
	string im1OutFilename;
	string im2OutFilename;
	int    rectifyType;  //0 -> STANDARD; not 0 -> HARTLEY

public:
	Test_StandAloneImageRectifier(void) {}
	~Test_StandAloneImageRectifier(void) {}

public:
	void test(const char* configFilename) {
		loadConfig(configFilename);
		
		IplImage* im1 = cvLoadImage(im1Filename.c_str(), 1);		
		if (im1 == NULL) error("im1 load failed!");
		IplImage* im2 = cvLoadImage(im2Filename.c_str(), 1);
		if (im2 == NULL) error("im2 load failed!");

		TwoViewFeatureTracker* tracker = loadTwoViewFeatureTracker(trackerParamsFilename.c_str());

		vector<CvPoint> p1s, p2s;
		tracker->compute(im1, im2, p1s, p2s);

		vector<CvPoint2D32f> x1s = cvPointsTo32fs(p1s);
		vector<CvPoint2D32f> x2s = cvPointsTo32fs(p2s);

		RANSAC_FMC ransac_fmc(0.05);
		Normalized_FMC nfmc(&ransac_fmc);

		FundamentalMatrix fm = nfmc.compute(x1s, x2s);

		CvMat* F = fm.getF();

		cout<<fm.toString()<<endl;

		ofstream ffs("temp/F.txt");
		ffs<<F;
		ffs.close();

		FundamentalMatrixErrorCalculator* pfmec = new PointLine_FMEC();
		FundamentalMatrixInlierOutlierModel fmiom(F, pfmec, 1.0);

		vector<CvPoint2D32f> ix1s, ix2s, ox1s, ox2s;
		fmiom.classify(x1s, x2s, ix1s, ix2s, ox1s, ox2s);

		CvMat* H1;
		CvMat* H2;

		if (rectifyType == 0) {
			TrueStereo_PBIR ts;
			ts.compute(F, cvGetSize(im1), ix1s, ix2s, &H1, &H2);
		} else {
			Hartley_PBIR ts;
			ts.compute(F, cvGetSize(im1), ix1s, ix2s, &H1, &H2);
		}

		IplImage* im1_H = cvCreateImage(cvGetSize(im1), 8, 3);
		IplImage* im2_H = cvCreateImage(cvGetSize(im2), 8, 3);

		cvWarpPerspective(im1, im1_H, H1);
		cvWarpPerspective(im2, im2_H, H2);
		
		cvSaveImage(im1OutFilename.c_str(), im1_H);
		cvSaveImage(im2OutFilename.c_str(), im2_H);

	}

	void error(const char* msg) {
		cout<<msg<<endl;
		exit(1);
	}

	void loadConfig(const char* configFilename) {
		ifstream ifs(configFilename);
		ifs>>trackerParamsFilename;
		ifs>>im1Filename;
		ifs>>im2Filename;
		ifs>>im1OutFilename;
		ifs>>im2OutFilename;
		ifs>>rectifyType;
		ifs.close();
	}

	TwoViewFeatureTracker* loadTwoViewFeatureTracker(const char* filename) {
		
		int cornersCount = 400;
		double qualityLevel = 0.02;
		double minDistance = 8;

		int halfWinSize = 4;
		int searchHalfWinSize = 3;
		int maxDifference = 20;
		int maxSearchDistance = 200;

		ifstream ifs(filename);
		ifs>>cornersCount>>qualityLevel>>minDistance;
		ifs>>halfWinSize>>searchHalfWinSize>>maxDifference>>maxSearchDistance;
		ifs.close();

		CornerDetector* pcd = new OpenCVCornerDetector(cornersCount, qualityLevel, minDistance);
		PointsOutFeatureMatcher* ppofm = new SimpleWindowPOFM(new SimpleWindowSSDFDC(halfWinSize, searchHalfWinSize, maxDifference), maxSearchDistance);

		return new TwoViewFeatureTracker(pcd, ppofm);
	}

};

		}
	}
}