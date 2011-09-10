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

using namespace std;
using namespace ferry::cv_geometry;
using namespace ferry::cv_geometry::io_util;

namespace ferry {
	namespace cv_geometry {
		namespace test {


class Test_CameraPositionEstimator
{
public:
	Test_CameraPositionEstimator(void) {
	}

	~Test_CameraPositionEstimator(void) {
	}

private:
	char dir[100];

	vector<string> vFile;

	CvMat* F;
	vector<CvPoint> cp1;
	vector<CvPoint> cp2;
	vector<CvPoint2D32f> x1s;
	vector<CvPoint2D32f> x2s;
	IplImage* im1;
	IplImage* im2;

	CvMat* H1;
	CvMat* H2;

	IplImage* im1_H;
	IplImage* im2_H;

	int type;

	static const int STANDARD = 0;
	static const int HARTLEY = 1;

	int index1;
	int index2;

public:
	void test() {
		index1 = 0;
		index2 = 1;

		loadData();

		draw_correspondences_image("cos", im1, im2, cp1, cp2);
		draw_correspondences_image("cos2", im1, cp1, cp2);

		drawEpipoleLines(im1, im2, F);
		
		save_correspondences("temp/cos1.jpg", im1, cp1, cp2);

		cvNamedWindow("im1", 1);
		cvShowImage("im1", im1);
		cvNamedWindow("im2", 1);
		cvShowImage("im2", im2);

		type = 0;

		if (type == STANDARD) {
			TrueStereo_PBIR ts;
			ts.compute(F, cvGetSize(im1), x1s, x2s, &H1, &H2);
		} else {
			Hartley_PBIR ts;
			ts.compute(F, cvGetSize(im1), x1s, x2s, &H1, &H2);
		}
		
		showTransformation(im1, im2, F, H1, H2);

		vector<CvPoint2D32f> tx1s, tx2s;
		tx1s = transformPoints(H1, x1s);
		tx2s = transformPoints(H2, x2s);
		draw_correspondences_image("trans cos1", im1_H, cvPointsFrom32fs(tx1s), cvPointsFrom32fs(tx2s));
		save_correspondences("temp/trans cos1.jpg", im1_H, cvPointsFrom32fs(tx1s), cvPointsFrom32fs(tx2s));

		double totalx = 0;
		double totaly = 0;
		double mindeltax = 10000;
		double maxdeltax = -10000;
		for (int i = 0; i < tx1s.size(); i++) {
			cout<<tx1s[i].x<<", "<<tx1s[i].y<<"  =>  "<<tx2s[i].x<<tx2s[i].y<<endl;
			double deltax = tx1s[i].x - tx2s[i].x;
			if (mindeltax > deltax) mindeltax = deltax;
			if (maxdeltax < deltax) maxdeltax = deltax;
			totalx += deltax;
			totaly += tx1s[i].y - tx2s[i].y;
		}
		double avgx = totalx / tx1s.size();
		double avgy = totaly / tx2s.size();
		double medianx = (mindeltax + maxdeltax) / 2;

		cout<<"avg x: "<<avgx<<endl;
		cout<<"avg y: "<<avgy<<endl;

		char cosfilename[300];
		sprintf(cosfilename, "temp/trans cos %d - %d (%.2f %.2f).jpg", dir, index1, index2, avgx, medianx);
		save_correspondences(cosfilename, im1_H, cvPointsFrom32fs(tx1s), cvPointsFrom32fs(tx2s));

	}

	void testCalibration() {
		index1 = 5;
		index2 = 7;

		loadData();

		save_correspondences("temp/cos.jpg", im1, im2, cp1, cp2);
		//draw_correspondences_image("cos2", im1, cp1, cp2);

		drawEpipoleLines(im1, im2, F);
		
		//cvNamedWindow("im1", 1);
		//cvShowImage("im1", im1);
		//cvNamedWindow("im2", 1);
		//cvShowImage("im2", im2);

		type = 0;

		TrueStereo_PBIR ts;
		ts.setAlpha(0.50);
		ts.setX0Disparity(45);
		//ts.setFYRatio(0.05);
		//ts.setPrincipleYRatio(-0.05);
		//ts.setPrincipleXRatio(0);
		ts.compute(F, cvGetSize(im1), x1s, x2s, &H1, &H2);
		
		vector<CvPoint2D32f> tx1s, tx2s;
		tx1s = transformPoints(H1, x1s);
		tx2s = transformPoints(H2, x2s);

		double mindeltax = 10000;
		double maxdeltax = -10000;
		for (int i = 0; i < tx1s.size(); i++) {
			//cout<<tx1s[i].x<<", "<<tx1s[i].y<<"  =>  "<<tx2s[i].x<<tx2s[i].y<<endl;
			double deltax = tx1s[i].x - tx2s[i].x;
			cout<<deltax<<endl;
			if (mindeltax > deltax) mindeltax = deltax;
			if (maxdeltax < deltax) maxdeltax = deltax;
		}

		cout<<"min: "<<mindeltax<<endl;
		cout<<"max: "<<maxdeltax<<endl;
		//showTransformation(im1, im2, F, H1, H2);

		CvMat* K0 = ts.getK0();
		CvMat* R0 = ts.getR0();
		CvMat* H1 = ts.getH1();

		ofstream ofs("C:/zj/vcprojects/ferrylibs/matlab/apps/KRHValues.m");
		toMatlabFile(ofs, K0, "K0");
		toMatlabFile(ofs, R0, "R0");
		toMatlabFile(ofs, H1, "H1");
	}

	void testWithOutImageShow(int index1, int index2) {
		
		cp1.clear();
		cp2.clear();
		LoadCorrespondPoints(index1, index2, cp1, cp2);

		if (cp1.size() < 15) {
			cout<<"points not enough!"<<endl;
			return;
		}

		x1s = cvPointsTo32fs(cp1);
		x2s = cvPointsTo32fs(cp2);

		F = NULL;
		LoadFM(index1, index2, &F);

		type = 0;

		cout<<index1<<" => "<<index2<<endl;
		cout<<"F: "<<F<<endl;

		const char* im1_file = vFile[index1].c_str();
		const char* im2_file = vFile[index2].c_str();

		im1 = cvLoadImage(im1_file, 1);
		im2 = cvLoadImage(im2_file, 1);

		if (type == STANDARD) {
			TrueStereo_PBIR ts;
			ts.compute(F, cvGetSize(im1), x1s, x2s, &H1, &H2);
		} else {
			Hartley_PBIR ts;
			ts.compute(F, cvGetSize(im1), x1s, x2s, &H1, &H2);
		}

		vector<CvPoint2D32f> tx1s, tx2s;
		tx1s = transformPoints(H1, x1s);
		tx2s = transformPoints(H2, x2s);

		double totalx = 0;
		double totaly = 0;
		double mindeltax = 10000;
		double maxdeltax = -10000;
		for (int i = 0; i < tx1s.size(); i++) {
			cout<<tx1s[i].x<<", "<<tx1s[i].y<<"  =>  "<<tx2s[i].x<<tx2s[i].y<<endl;
			double deltax = tx1s[i].x - tx2s[i].x;
			if (mindeltax > deltax) mindeltax = deltax;
			if (maxdeltax < deltax) maxdeltax = deltax;
			totalx += deltax;
			totaly += tx1s[i].y - tx2s[i].y;
		}
		double avgx = totalx / tx1s.size();
		double avgy = totaly / tx2s.size();
		double medianx = (mindeltax + maxdeltax) / 2;

		cout<<"avg x: "<<avgx<<endl;
		cout<<"avg y: "<<avgy<<endl;

		im1_H = cvCreateImage(cvGetSize(im1), 8, 3);

		cvWarpPerspective(im1, im1_H, H1);

		char cosfilename[300];
		sprintf(cosfilename, "%s\\trans cos %d - %d (%.2f %.2f).jpg", dir, index1, index2, avgx, medianx);
		save_correspondences(cosfilename, im1_H, cvPointsFrom32fs(tx1s), cvPointsFrom32fs(tx2s));

		cvReleaseImage(&im1);
		cvReleaseImage(&im2);
		cvReleaseImage(&im1_H);

	}

	void test2() {
		sprintf(dir, "%s", "C:\\zj\\research\\computer vision\\3d_modeling\\data\\cam position\\diningroom1");

		fvLoadFileName(vFile);
		
		for (int i = 3; i < vFile.size() - 1; i++) {
			for (int j = i + 1; j < vFile.size(); j++) {
				if (j - i >= 5) continue;
				testWithOutImageShow(i, j);
			}
		}
	}

	void loadData() {
		sprintf(dir, "%s", "C:\\zj\\research\\computer vision\\3d_modeling\\data\\cam position\\house1");
		
		F = NULL;
		LoadFM(index1, index2, &F);

		LoadCorrespondPoints(index1, index2, cp1, cp2);

		x1s = cvPointsTo32fs(cp1);
		x2s = cvPointsTo32fs(cp2);

		cout<<"x1s: "<<endl;
		cout<<x1s<<endl;
		cout<<"x2s: "<<endl;
		cout<<x2s<<endl;

		cout<<"F: "<<endl;
		cout<<F<<endl;

		fvLoadFileName(vFile);

		const char* im1_file = vFile[index1].c_str();
		const char* im2_file = vFile[index2].c_str();

		im1 = cvLoadImage(im1_file, 1);
		im2 = cvLoadImage(im2_file, 1);
	}

	void LoadCorrespondPoints(int index1, int index2, vector<CvPoint> &cp1, vector<CvPoint> &cp2)
	{
		FILE *fp;
		int x1, y1, x2, y2;
		char szPath[MAX_PATH];
		sprintf(szPath, "%s\\data\\cp%03d%03d.txt", dir, index1, index2);
		if(NULL != (fp = fopen(szPath, "r")))
		{
			while(EOF != fscanf(fp, "%d, %d, %d, %d", &x1, &y1, &x2, &y2))
			{
				cp1.push_back(cvPoint(x1, y1));
				cp2.push_back(cvPoint(x2, y2));
			}
			fclose(fp);
		}
	}

	void LoadFM(int index1, int index2, CvMat **fundamental_matrix)
	{
		if(*fundamental_matrix == NULL)
			*fundamental_matrix = cvCreateMat(3, 3, CV_64F);

		int i;
		FILE *fp;
		char fmfile[MAX_PATH];

		sprintf(fmfile, "%s\\data\\fm%03d%03d.txt", dir, index1, index2);
		
		fp = fopen(fmfile, "r");
		double fme;
		if(fp != NULL)
		{
			for(i=0; i<9; i++)	
			{
				fscanf(fp, "%lf\t", &fme);
				cvmSet(*fundamental_matrix, i/3, i%3, fme);
			}
			fclose(fp);
		}
		else
		{
			return;
		}
	}

	void fvLoadFileName(vector<string> &vFile)
	{
		int i;
		FILE *fp;
		char szPath[MAX_PATH], szFile[MAX_PATH];
		sprintf(szPath, "%s\\data\\files.txt", dir);
		int dummy;
		if(NULL != (fp = fopen(szPath, "r")))
		{
			while(EOF != fscanf(fp, "%d, %s", &dummy, &szFile))
			{
				vFile.push_back(dir + string("\\") + string(szFile));
			}
			fclose(fp);
		}
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

		cvNamedWindow("im1", 1);
		cvShowImage("im1", im1);
		cvNamedWindow("im2", 1);
		cvShowImage("im2", im2);

		im1_H = cvCreateImage(cvGetSize(im1), 8, 3);
		im2_H = cvCreateImage(cvGetSize(im2), 8, 3);

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