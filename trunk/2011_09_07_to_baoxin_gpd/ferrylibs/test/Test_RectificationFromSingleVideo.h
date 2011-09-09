#pragma once

#include <iostream>
#include <stdio.h>
#include <fstream>

#include "highgui.h"

#include <ferry/feature_tracking/TwoViewFeatureTracker.h>

#include <ferry/cv_geometry/DLT_FMC.h>
#include <ferry/cv_geometry/RANSAC_FMC.h>
#include <ferry/cv_geometry/Normalized_FMC.h>
#include <ferry/cv_geometry/OpenCV_FMC.h>
#include <ferry/cv_geometry/IOUtil.h>
#include <ferry/cv_geometry/DrawingUtil.h>
#include <ferry/cv_geometry/PointCorrespondences.h>

#include <ferry/cv_geometry/ImageRectifier.h>
#include <ferry/cv_geometry/TwoViewGeometryUtil.h>
#include <ferry/cv_geometry/DrawingUtil.h>
#include <ferry/cv_geometry/IOUtil.h>
#include <ferry/cv_geometry/CrossOpticalAxes_PBIR.h>

using namespace std;
using namespace ferry::cv_geometry;
using namespace ferry::cv_geometry::io_util;
using namespace ferry::feature_tracking;

namespace ferry {
	namespace cv_geometry {
		namespace test {


class Test_RectificationFromSingleVideo
{
public:
	Test_RectificationFromSingleVideo(void) {
	}

	~Test_RectificationFromSingleVideo(void) {
	}

public:
	void test() {
		char* srcdir = "H:/transfer/Side-Looking 1(straight)12-03-06 2";
		char* dstdir = "H:/transfer/Out Side-Looking 1(straight)12-03-06 2";

		CornerDetector* pcd = new OpenCVCornerDetector(400, 0.02, 8);
		//CornerDetector* pcd = new BlockCornerDetector(100, 10, 0.02, 15);
		PointsOutFeatureMatcher* ppofm = new SimpleWindowPOFM(new SimpleWindowSSDFDC(4, 3, 20), 200);
		//PointsOutFeatureMatcher* ppofm = new SimpleWindowPOFM(new SimpleWindowSSDFDC(5, 3, 30), 50);

		TwoViewFeatureTracker tvft(pcd, ppofm);

		char file1[500];
		char file2[500];
		//char* file1 = "data/olympus/im_134.bmp";
		//char* file2 = "data/olympus/im_136.bmp";

		int index1 = 5;
		int index2 = 7;

		sprintf(file1, "%s/im_%04d.bmp", srcdir, index1);
		sprintf(file2, "%s/im_%04d.bmp", srcdir, index2);

		IplImage* im1 = cvLoadImage(file1, 1);
		IplImage* im2 = cvLoadImage(file2, 1);

		IplImage* im1_w = cvCloneImage(im1);
		IplImage* im2_w = cvCloneImage(im2);
		IplImage* mixim = cvCloneImage(im1);

#if 0
		shift(im1, im1_w, 5);
#else
		//CvRect middleThird = cvRect(0, im1->height/3, im1->width, im1->height/3);
		//cvSetImageROI(im1, middleThird);
		//cvSetImageROI(im2, middleThird);

		vector<CvPoint> x1s, x2s;
		tvft.compute(im1, im2, x1s, x2s);

		PointCorrespondences pc;
		char pc_file[500];
		sprintf(pc_file, "%s/mpicked.txt", srcdir);
		ifstream pcifs(pc_file);
		pcifs>>pc;
		pcifs.close();
		//x1s = cvPointsFrom32fs(pc.x1s);
		//x2s = cvPointsFrom32fs(pc.x2s);

		draw_correspondences_image("cos1", im1, x1s, x2s);
		draw_correspondences_image("cos2", im1, im2, x1s, x2s);
		IplImage* cos1 = create_correspondences_image(im1, x1s, x2s);
		char buf[400];
		sprintf(buf, "%s/temp/im_%04d_%04d_cos1.bmp", srcdir, index1, index2);
		cvSaveImage(buf, cos1);

		DLT_FMC dltfmc;
		//Normalized_FMC nfmc(&dltfmc);

		RANSAC_FMC ransac_fmc(0.01);
		Normalized_FMC nfmc(&ransac_fmc);

		//OpenCV_FMC nfmc(CV_FM_RANSAC, 1, 0.99);
		//OpenCV_FMC nfmc(CV_FM_LMEDS, 1, 0.99);

		//FundamentalMatrix fm = dltfmc.compute(x1s, x2s);
		FundamentalMatrix fm = nfmc.compute(cvPointsTo32fs(x1s), cvPointsTo32fs(x2s));

		cout<<fm.toString()<<endl;

		FundamentalMatrixErrorCalculator* pfmec = new PointLine_FMEC();
		//FundamentalMatrixErrorCalculator* pfmec = new SimpleMultiply_FMEC();
		FundamentalMatrixBatchErrorCalculator fmbec(fm.getF(), pfmec);
		double error = fmbec.compute(cvPointsTo32fs(x1s), cvPointsTo32fs(x2s));
		cout<<"error: "<<error<<endl;

		FundamentalMatrixInlierOutlierModel fmiom(fm.getF(), pfmec, 1.0);
		vector<CvPoint2D32f> ix1s, ix2s, ox1s, ox2s;
		fmiom.classify(cvPointsTo32fs(x1s), cvPointsTo32fs(x2s), ix1s, ix2s, ox1s, ox2s);

		draw_correspondences_image("inliers cos1", im1, cvPointsFrom32fs(ix1s), cvPointsFrom32fs(ix2s));
		IplImage* inliers_cos1 = create_correspondences_image(im1, cvPointsFrom32fs(ix1s), cvPointsFrom32fs(ix2s));
		sprintf(buf, "%s/temp/im_%04d_%04d_inliers_cos1.bmp", srcdir, index1, index2);
		cvSaveImage(buf, inliers_cos1);

		CvMat* F = fm.getF();
		drawEpipoleLines(im1, im2, F);

		cvNamedWindow("im1", 1);
		cvNamedWindow("im2", 1);
		cvShowImage("im1", im1);
		cvShowImage("im2", im2);
		sprintf(buf, "%s/temp/im_%04d_F.bmp", srcdir, index1);
		cvSaveImage(buf, im1);
		sprintf(buf, "%s/temp/im_%04d_F.bmp", srcdir, index2);
		cvSaveImage(buf, im2);

		CvMat* H1;
		CvMat* H2;

		TrueStereo_PBIR ts;
		ts.compute(F, cvGetSize(im1), ix1s, ix2s, &H1, &H2);

		IplImage* im1_H = cvCreateImage(cvGetSize(im1), 8, 3);
		IplImage* im2_H = cvCreateImage(cvGetSize(im2), 8, 3);

		cvWarpPerspective(im1, im1_H, H1);
		cvWarpPerspective(im2, im2_H, H2);
		cvNamedWindow("im1 H", 1);
		cvShowImage("im1 H", im1_H);
		cvNamedWindow("im2 H", 1);
		cvShowImage("im2 H", im2_H);
		sprintf(buf, "%s/temp/im_%04d_rectified.bmp", srcdir, index1);
		cvSaveImage(buf, im1_H);
		sprintf(buf, "%s/temp/im_%04d_rectified.bmp", srcdir, index2);
		cvSaveImage(buf, im2_H);

		im1_w = im1_H;
		im2_w = im2_H;
#endif
		mix(im1_w, im2_w, mixim);
		cvNamedWindow("mix", 1);
		cvShowImage("mix", mixim);
	}

	void shift(IplImage* im, IplImage* im_w, double transv) {
		double mh[] = {1, 0, transv,  0, 1, 0,  0, 0, 1};
		CvMat TH = cvMat(3, 3, CV_64FC1, mh);

		cvWarpPerspective(im, im_w, &TH);
	}

	void mix(IplImage* im1, IplImage* im2, IplImage* mixim) {
		cvAddWeighted(im1, 0.5, im2, 0.5, 0, mixim);
	}
};


		}
	}
}