#pragma once

#include <iostream>
#include <stdio.h>
#include <fstream>

#include "highgui.h"

#include <ferry/cv_geometry/ImageRectifier.h>
#include <ferry/cv_geometry/TwoViewGeometryUtil.h>
#include <ferry/cv_geometry/DrawingUtil.h>
#include <ferry/cv_geometry/IOUtil.h>
#include <ferry/cv_geometry/CrossOpticalAxes_PBIR.h>

#include "Test_MSVideo.h"

using namespace std;
using namespace ferry::cv_geometry;
using namespace ferry::cv_geometry::io_util;

namespace ferry {
	namespace cv_geometry {
		namespace test {

class Test_ImageRectifier
{
public:
	Test_ImageRectifier() {
		type = 0;
		scaleForLarge = false;
		drawEpipolarLines = false;
	}

private:
	CvMat* F;
	vector<CvPoint2D32f> x1s;
	vector<CvPoint2D32f> x2s;
	IplImage* im1;
	IplImage* im2;

	CvMat* H1;
	CvMat* H2;

	int type;

	bool drawEpipolarLines;

	bool scaleForLarge;
	double scale;

public:
	static const int STANDARD = 0;
	static const int HARTLEY = 1;
	void setRectificationType(int type) {
		this->type = type;
	}

	void setScaleForLarge(double scale) {
		scaleForLarge = true;
		this->scale = scale;
	}

	void setDrawEpipolarLines() {
		this->drawEpipolarLines = true;
	}

	void test() {
		//test_MS_WithCameraMatrix();
		
		testWithF();
	}

	void loadData() {
		ifstream ifs("temp/F.txt");
		ifs>>&F;
		ifs.close();

		char* file1 = "temp/ix1s.txt";
		char* file2 = "temp/ix2s.txt";


		ifstream ifs1(file1);
		ifs1>>x1s;
		ifs1.close();

		ifstream ifs2(file2);
		ifs2>>x2s;
		ifs2.close();

		cout<<"x1s: "<<endl;
		cout<<x1s<<endl;
		cout<<"x2s: "<<endl;
		cout<<x2s<<endl;

		//char* im1_file = "data/MS_Video/dancing/3_jpg_small.jpg";
		//char* im2_file = "data/MS_Video/dancing/4_jpg_small.jpg";

		char* im1_file = "data/olympus/1_small.jpg";
		char* im2_file = "data/olympus/3_small.jpg";

		//char* im1_file = "data/simulation/view_3.bmp";
		//char* im2_file = "data/simulation/view_1.bmp";

		im1 = cvLoadImage(im1_file, 1);
		im2 = cvLoadImage(im2_file, 1);
	}

	void testWithF() {
		
		loadData();

		if (type == STANDARD) {
			TrueStereo_PBIR ts;
			ts.compute(F, cvGetSize(im1), x1s, x2s, &H1, &H2);
		} else {
			Hartley_PBIR ts;
			ts.compute(F, cvGetSize(im1), x1s, x2s, &H1, &H2);
		}
		
		showTransformation(im1, im2, F, H1, H2);

/*
		IplImage* im1_w = cvLoadImage("temp/im1_w.jpg", 1);
		vector<CvPoint2D32f> tx1s, tx2s;
		tx1s = transformPoints(H1, x1s);
		tx2s = transformPoints(H2, x2s);
		for (int i = 0; i < tx1s.size(); i++) {
			cout<<i<<" : "<<tx1s[i].x - tx2s[i].x<<", "<<tx1s[i].y - tx2s[i].y<<endl;
		}
		::draw_correspondences_image("trans cos1", im1_w, cvPointsFrom32fs(tx1s), cvPointsFrom32fs(tx2s));
*/
		ofstream ofs("temp/F_H.txt");
		ofs<<H1<<endl;
		ofs<<H2<<endl;
		ofs.close();

		if (scaleForLarge) {
			CvMat* LH1 = ::transHWithScale(H1, scale);
			CvMat* LH2 = ::transHWithScale(H2, scale);
			IplImage* Lim1 = cvLoadImage("data/olympus/1.jpg", 1);
			IplImage* Lim2 = cvLoadImage("data/olympus/3.jpg", 1);

			IplImage* im1_H = cvCreateImage(cvGetSize(Lim1), 8, 3);
			IplImage* im2_H = cvCreateImage(cvGetSize(Lim2), 8, 3);

			cvWarpPerspective(Lim1, im1_H, LH1);
			cvWarpPerspective(Lim2, im2_H, LH2);

			cvNamedWindow("Lim1 H", 1);
			cvShowImage("Lim1 H", im1_H);
			cvNamedWindow("Lim2 H", 1);
			cvShowImage("Lim2 H", im2_H);

			cvSaveImage("temp/1_w.jpg", im1_H);
			cvSaveImage("temp/2_w.jpg", im2_H);

			ofstream ofs("temp/L_H.txt");
			ofs<<LH1<<endl;
			ofs<<LH2<<endl;
			ofs.close();

		}
	}

	void continueRectify(bool withLargeImage = false) {

		CvMat* H1;
		CvMat* H2;

		cout<<"Load H1 and H2 ......."<<endl;

		char* Hfile = "temp/F_H.txt";
		if (withLargeImage) Hfile = "temp/L_H.txt";

		ifstream ifs(Hfile);
		ifs>>&H1;
		ifs>>&H2;
		ifs.close();

		cout<<"H1: "<<H1<<endl;
		cout<<"H2: "<<H2<<endl;

		char* dir = "./data/Jan-06-video";
		char* im1_d = "Channel1_High";
		char* im2_d = "Channel2_High";
		char* out_im1_d = "Out_Channel1_High";
		char* out_im2_d = "Out_Channel2_High";
		
		char im1_dir[100];
		char im2_dir[100];
		char out_im1_dir[100];
		char out_im2_dir[100];

		sprintf(im1_dir, "%s/%s", dir, im1_d);
		sprintf(im2_dir, "%s/%s", dir, im2_d);
		sprintf(out_im1_dir, "%s/%s", dir, out_im1_d);
		sprintf(out_im2_dir, "%s/%s", dir, out_im2_d);
		rectifyImagesWithSameH(H1, H2, im1_dir, im2_dir, out_im1_dir, out_im2_dir);
	}

	//use H1 and H2 computed in testWithF function to rectify other images
	//filename convention: im_i.jpg i = 0 ~ n
	void rectifyImagesWithSameH(CvMat* H1, CvMat* H2, const char* im1_dir, const char* im2_dir, const char* out_im1_dir, const char* out_im2_dir) {
		char im1_file[100];
		char im2_file[100];
		char out_im1_file[100];
		char out_im2_file[100];

		IplImage* im1_temp;
		int MAX_IMAGE_NUMBERS = 10000;
		for (int i = 0; i < MAX_IMAGE_NUMBERS; i++) {
			sprintf(im1_file, "%s/im_%d.jpg", im1_dir, i);
			sprintf(im2_file, "%s/im_%d.jpg", im2_dir, i);
			sprintf(out_im1_file, "%s/im_%d.jpg", out_im1_dir, i);
			sprintf(out_im2_file, "%s/im_%d.jpg", out_im2_dir, i);
			if ((im1_temp = cvLoadImage(im1_file, 1)) != NULL) {
				rectifyImageWithSameH(H1, H2, im1_file, im2_file, out_im1_file, out_im2_file);
				cvReleaseImage(&im1_temp);
				cout<<i<<" finished..............."<<endl;
			} else return;
		}
	}

	//rectify single image
	void rectifyImageWithSameH(CvMat* H1, CvMat* H2, const char* im1_file, const char* im2_file, const char* out_im1_file, const char* out_im2_file) {
		IplImage* im1_src = cvLoadImage(im1_file, 1);
		IplImage* im2_src = cvLoadImage(im2_file, 1);

		IplImage* im1_H = cvCreateImage(cvGetSize(im1_src), 8, 3);
		IplImage* im2_H = cvCreateImage(cvGetSize(im2_src), 8, 3);

		cvWarpPerspective(im1_src, im1_H, H1);
		cvWarpPerspective(im2_src, im2_H, H2);

		cvSaveImage(out_im1_file, im1_H);
		cvSaveImage(out_im2_file, im2_H);

		cvReleaseImage(&im1_src);
		cvReleaseImage(&im1_H);
		cvReleaseImage(&im2_src);
		cvReleaseImage(&im2_H);
	}

	void test_MS_WithCameraMatrix() {
		Test_MSVideo tmsv;
		tmsv.readCalibrationFile("data/MS_Video/dancing/calibParams-breakdancers.txt");
		
		//char* file1 = "data/MS_Video/dancing/color-cam3-f000.bmp";
		//char* file2 = "data/MS_Video/dancing/color-cam4-f000.bmp";
		char* file1 = "data/MS_Video/dancing/3_jpg_small.jpg";
		char* file2 = "data/MS_Video/dancing/4_jpg_small.jpg";

		IplImage* im1 = cvLoadImage(file1, 1);
		IplImage* im2 = cvLoadImage(file2, 1);

		Camera* pcam1 = tmsv.cams[3];
		Camera* pcam2 = tmsv.cams[4];

		pcam1->transWithScale(0.4);
		pcam2->transWithScale(0.4);

		pcam1->transWithFlipY(im1->height);
		pcam2->transWithFlipY(im2->height);

		CvMat* H1;
		CvMat* H2;

		ImageRectifier::estimateHomography(pcam1,pcam2, &H1, &H2);
		//ImageRectifier::simpleEstimateHomography(pcam1,pcam2, &H1, &H2);

		float theta = 1.5 * 3.1415 / 180;
		double mh1[] = {cos(theta), 0, -sin(theta),  0, 1, 0,  sin(theta), 0, cos(theta)};
		CvMat MH1 = cvMat(3, 3, CV_64FC1, mh1);
		theta = -theta;
		double mh2[] = {cos(theta), 0, -sin(theta),  0, 1, 0,  sin(theta), 0, cos(theta)};
		CvMat MH2 = cvMat(3, 3, CV_64FC1, mh2);

		cout<<"MH1: "<<&MH1<<endl;
		CvMat* K1 = pcam1->getK();
		CvMat* K2 = pcam2->getK();
		CvMat* TH1 = matMul(matMul(K1, matMul(&MH2, inv(K1))), H1);
		CvMat* TH2 = matMul(matMul(K2, matMul(&MH1, inv(K2))), H2);

		cout<<"H1: "<<H1<<endl;
		cout<<"H2: "<<H2<<endl;
		
		CvMat* e1 = CvMatUtil::matMul(pcam1->getP(), pcam2->getHC());
		CvMat* e2 = CvMatUtil::matMul(pcam2->getP(), pcam1->getHC());

		cout<<"e1: "<<e1<<endl;
		cout<<"e2: "<<e2<<endl;

		cout<<"H1 * e1 "<<matMul(H1, pcam1->getP(), pcam2->getHC())<<endl;
		cout<<"H2 * e2 "<<matMul(H2, pcam2->getP(), pcam1->getHC())<<endl;


		CvMat* F = TwoViewGeometryUtil::calcF(pcam1->getP(), pcam2->getP());

		showTransformation(im1, im2, F, TH1, TH2);

		//CvMat* nF = transFWithFlipY(F, im1->height);

		//CvMat* nH1 = transHWithFlipY(H1, im1->height);
		//CvMat* nH2 = transHWithFlipY(H2, im1->height);

		//showTransformation(im1, im2, nF, nH1, nH2);


		//=====================================
/*		char* file1_small = "data/MS_Video/dancing/3_jpg_small.jpg";
		char* file2_small = "data/MS_Video/dancing/4_jpg_small.jpg";
		IplImage* im1_small = cvLoadImage(file1_small, 1);
		IplImage* im2_small = cvLoadImage(file2_small, 1);

		CvMat* nF_small = transFWithScale(nF, 0.4);

		ofstream f_ofs("temp/G_F.txt");
		f_ofs<<nF_small<<endl;
		f_ofs.close();

		CvMat* nH1_small = transHWithScale(nH1, 0.4);
		CvMat* nH2_small = transHWithScale(nH2, 0.4);

		ofstream ofs("temp/C_H.txt");
		ofs<<nH1_small<<endl;
		ofs<<nH2_small<<endl;
		ofs.close();

		showTransformation(im1_small, im2_small, nF_small, nH1_small, nH2_small);
*/	}

	void testCross() {
		loadData();

		TrueStereo_PBIR ts;
		//Hartley_PBIR ts;
		ts.compute(F, cvGetSize(im1), x1s, x2s, &H1, &H2);

		//////////////////////////////////////////////////////
		CvMat* K = ts.getK0();
		cout<<"K: "<<K<<endl;
		CvMat* CH1;
		CvMat* CH2;
		float tanTheta = -tan(0 * 3.14/180);
		cout<<"tanTheta: "<<tanTheta<<endl;
		float f = cvmGet(K, 0, 0);
		cout<<"f: "<<f<<endl;
		float d0 = 24;
		cout<<"d0/2f: "<<d0/2/f<<endl;
		CrossOpticalAxes_PBIR coa;
		coa.compute(K, tanTheta, &CH1, &CH2);

		CvMat* TH1 = getIdentity();
		cvmSet(TH1, 0, 2, 40);

		CvMat* COH1 = matMul(CH1, TH1, H1);
		CvMat* COH2 = matMul(CH2, H2);
		showTransformation(im1, im2, F, COH1, COH2);

		IplImage* im1_w = cvLoadImage("temp/im1_w.jpg", 1);
		vector<CvPoint2D32f> tx1s, tx2s;
		tx1s = transformPoints(COH1, x1s);
		tx2s = transformPoints(COH2, x2s);
		::draw_correspondences_image("trans cos1", im1_w, cvPointsFrom32fs(tx1s), cvPointsFrom32fs(tx2s));
		/////////////////////////////////////////////

	}

	void showTransformation(IplImage* im1, IplImage* im2, CvMat* F, CvMat* H1, CvMat* H2) {

		CvMat* e2 = leftNull(F);
		cout<<"e2: "<<e2<<endl;
		cout<<"H2: "<<H2<<endl;
		cout<<"H2 * e2: "<<matMul(H2, e2)<<endl;
		CvMat* e1 = null(F);
		cout<<"e1: "<<e1<<endl;
		cout<<"H1: "<<H1<<endl;
		cout<<"H1 * e1: "<<matMul(H1, e1)<<endl;

		if (drawEpipolarLines) {
			::drawEpipoleLines(im1, im2, F);
		}

		cvNamedWindow("im1", 1);
		cvShowImage("im1", im1);
		cvNamedWindow("im2", 1);
		cvShowImage("im2", im2);

		IplImage* im1_H = cvCreateImage(cvGetSize(im1), 8, 3);
		IplImage* im2_H = cvCreateImage(cvGetSize(im2), 8, 3);

		cvWarpPerspective(im1, im1_H, H1);
		cvWarpPerspective(im2, im2_H, H2);

		cvNamedWindow("im1 H", 1);
		cvShowImage("im1 H", im1_H);
		cvNamedWindow("im2 H", 1);
		cvShowImage("im2 H", im2_H);

		cvSaveImage("temp/im1.jpg", im1);
		cvSaveImage("temp/im2.jpg", im2);

		cvSaveImage("temp/im1_w.jpg", im1_H);
		cvSaveImage("temp/im2_w.jpg", im2_H);


	}
};

		}
	}
}