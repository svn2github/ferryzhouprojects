#pragma once

#include <iostream>
#include <fstream>

#include <ferry/cv_geometry/DLT_HC.h>
#include <ferry/cv_geometry/TwoViewSimulation.h>
#include <ferry/cv_geometry/DrawingUtil.h>
#include <ferry/cv_geometry/BasicUtil.h>
#include <ferry/cv_geometry/DLT_HC.h>

#include "NormalizedHomographyBasedGPD.h"
#include "REstimator.h"
#include "VerticalLineDetector.h"
#include "VLinesBasedREstimator.h"
#include "Util.h"

using namespace std;
using namespace ferry::cv_geometry;

#define OLD 0

class Test_GroundPlaneDetection
{

private:
	CvMat* K;
	vector<CvPoint2D32f> gxs;
	vector<CvPoint2D32f> ixs;
	vector<CvPoint2D32f> x1s;
	vector<CvPoint2D32f> x2s;
	IplImage* im1;
	IplImage* im2;

public:

	void testReal() {
		char* dir = "data/robot";

		char im1_file[100];
		sprintf(im1_file, "%s/1_small.jpg", dir);
		char im2_file[100];
		sprintf(im2_file, "%s/3_small.jpg", dir);

		im1 = cvLoadImage(im1_file, 1);
		im2 = cvLoadImage(im2_file, 1);

		char Kfile[100];
		sprintf(Kfile, "%s/K.txt", dir);
		ifstream ifsK(Kfile);
		ifsK>>&K;
		ifsK.close();

		char x1s_file[100];
		sprintf(x1s_file, "%s/ix1s.txt", dir);
		char x2s_file[100];
		sprintf(x2s_file, "%s/ix2s.txt", dir);

		ifstream ifs1(x1s_file);
		ifs1>>x1s;
		ifs1.close();

		ifstream ifs2(x2s_file);
		ifs2>>x2s;
		ifs2.close();

		cout<<"x1s: "<<endl;
		cout<<x1s<<endl;
		cout<<"x2s: "<<endl;
		cout<<x2s<<endl;

		for (size_t i = 0; i < x1s.size(); i++) {
			cout<<"x: "<<x1s[i].x<<" -> "<<(x1s[i].x - x2s[i].x)<<", "<<(x1s[i].y - x2s[i].y)<<endl;
		}

		//loadData();
#if OLD
		REstimator re;

		char* gfile = "data/robot/gxs.txt";
		char* ifile = "data/robot/ixs.txt";

		ifstream gfs(gfile);
		gfs>>gxs;
		gfs.close();

		ifstream kfs(ifile);
		kfs>>ixs;
		kfs.close();

		CvMat* R1 = re.compute(gxs, ixs, K);
		cout<<"R1: "<<R1<<endl;
		CvMat* R2 = R1;
#else
/*		CvMat* R1;
		ifstream ifs("data/robot/VR.txt");
		ifs>>&R1;
		cout<<"R1: "<<R1<<endl;
		ifs.close();
		CvMat* R2 = R1;
*/
		CvMat* R1 = inv(getR(im1));
		CvMat* R2 = inv(getR(im2));

		cout<<"R1: "<<R1<<endl;
		cout<<"R2: "<<R2<<endl;

#endif

		CvMat* H = matMul(K, inv(R1), inv(K));
		//IplImage* dst = cvCloneImage(im1);
		CvSize size = cvGetSize(im1);
		IplImage* dst = cvCreateImage(size, 8, 3);
		cvWarpPerspective(im1, dst, H);
		cvNamedWindow("dst", 1);
		cvShowImage("dst", dst);
		if (OLD) cvSaveImage("results/im1_R1_old.bmp", dst);
		else cvSaveImage("results/im1_R1_new.bmp", dst);

		IplImage* dst2 = cvCreateImage(size, 8, 3);
		cvWarpPerspective(im2, dst2, H);
		cvNamedWindow("dst2", 1);
		cvShowImage("dst2", dst2);
		if (!OLD) cvSaveImage("results/im2_R2_new.bmp", dst2);

		CvMat* M1 = matMul(K, R1);
		CvMat* M2 = matMul(K, R2);
		NormalizedHomographyBasedGPD gpd(x1s, x2s, M1, M2, 0.012);
		//gpd.fitModel(nngH);
		gpd.compute();


		cout<<"is: "<<endl;
		vector<int> is = gpd.getis();
		
		vector<CvPoint2D32f> ix1s, ix2s, ox1s, ox2s;
		for (size_t i = 0; i < is.size(); i++) {
			ix1s.push_back(x1s[is[i]]);
			ix2s.push_back(x2s[is[i]]);
		}
		vector<int> os = gpd.getos();
		for (size_t i = 0; i < os.size(); i++) {
			ox1s.push_back(x1s[os[i]]);
			ox2s.push_back(x2s[os[i]]);
		}

		::draw_correspondences_image("cos", im1, cvPointsFrom32fs(x1s), cvPointsFrom32fs(x2s));
		drawGroundObstaclePointsImage("ground", im1, cvPointsFrom32fs(ix1s), cvPointsFrom32fs(ox1s));
		IplImage* nim = ::createGroundObstaclePointsImage(im1, cvPointsFrom32fs(ix1s), cvPointsFrom32fs(ox1s));
		cvSaveImage("results/NH_ground.jpg", nim);
		IplImage* cos = ::create_correspondences_image(im1, cvPointsFrom32fs(x1s), cvPointsFrom32fs(x2s));
		cvSaveImage("results/cos.jpg", cos);
		IplImage* pim2 = ::createCornersImage(im2, cvPointsFrom32fs(x2s));
		cvSaveImage("results/pim2.jpg", pim2);

		CvMat* NH = gpd.getNH();
		if (NH == NULL) return;
		CvMat* HH = matMul(M2, NH, inv(M1));
		//HH = scale(HH, -1);
		//HH = getIdentity();
		cout<<"H: "<<H<<endl;
		cout<<"K: "<<K<<endl;
		cout<<"R1: "<<R1<<endl;
		cout<<"K R1: "<<matMul(K, R1)<<endl;
		cout<<"M1: "<<M1<<endl;
		cout<<"NH: "<<NH<<endl;
		cout<<"HH: "<<HH<<endl;

		IplImage* diff = compare(im2, im1, inv(HH));
		cvSaveImage("results/diff.jpg", diff);

		double mh[] = {1, 0, 0, 0, 0, 1, 0, 1, 0};
		CvMat GR = cvMat(3, 3, CV_64FC1, mh);
		double mh2[] = {1, 0, 1, 0, 1, 1, 0, 0, 0.01};
		CvMat GK = cvMat(3, 3, CV_64FC1, mh2);
		CvMat* GH = matMul(matMul(&GK, &GR), inv(R1), inv(K));
		//CvMat* GH = matMul(K, inv(R1), inv(K));
		//IplImage* ground = cvCloneImage(im1);
		IplImage* ground = cvCreateImage(cvSize(500, 500), 8, 3);
		cout<<"GR: "<<&GR<<endl;
		cout<<"GH: "<<GH<<endl;

		double mp[] = {0, 120, 1};
		CvMat MP = cvMat(3, 1, CV_64FC1, mp);
		cout<<"mmp: "<<hmatToPoint2D(matMul(GH, &MP))<<endl;

		double mp2[] = {0, im1->height, 1};
		CvMat MP2 = cvMat(3, 1, CV_64FC1, mp2);
		cout<<"mmp2: "<<hmatToPoint2D(matMul(GH, &MP2))<<endl;
		double mp4[] = {im1->width, im1->height, 1};
		CvMat MP4 = cvMat(3, 1, CV_64FC1, mp4);
		cout<<"mmp4: "<<hmatToPoint2D(matMul(GH, &MP4))<<endl;
		double mp3[] = {im1->width, 120, 1};
		CvMat MP3 = cvMat(3, 1, CV_64FC1, mp3);
		cout<<"mmp3: "<<hmatToPoint2D(matMul(GH, &MP3))<<endl;

		//cvSetImageROI(im1, cvRect(0, 200, im1->width, im1->height));
		//cvWarpPerspective(im1, ground, GH);
		//cvNamedWindow("ground2", 1);
		//cvShowImage("ground2", ground);

	}

	CvMat* getR(IplImage* im1) {
		IplImage* gray1 = cvCreateImage(cvGetSize(im1), 8, 1);
		cvCvtColor(im1, gray1, CV_BGR2GRAY);
		VerticalLineDetector vld(CV_HOUGH_STANDARD, 1, CV_PI/360, 80, 0, 0);
		vector<RhoThetaLine> vlines = vld.process(gray1);
		vector<CvMat*> lines = linesToMats(vlines);
		VLinesBasedREstimator re;
		ToVerticalR vr = re.computeVR(K, lines);

		CvMat* R1 = vr.getR();
		return cloneMat(R1);
	}

	vector<CvMat*> linesToMats(vector<RhoThetaLine>& vlines) {
		vector<CvMat*> lines;
		for (size_t i = 0; i < vlines.size(); i++) {
			CvMat* M = cvCreateMat(3, 1, CV_64FC1);
			double mh[] = {cos(vlines[i].theta), sin(vlines[i].theta), -vlines[i].rho};
			cvSetData(M, mh, CV_AUTOSTEP);
			cout<<"M: "<<M<<endl;
			lines.push_back(cloneMat(M));
		}
		return lines;
	}
	void loadData() {
		ifstream ifs("data/robot/K.txt");
		ifs>>&K;
		ifs.close();
		
		char* gfile = "data/robot/gxs.txt";
		char* ifile = "data/robot/ixs.txt";

		ifstream gfs(gfile);
		gfs>>gxs;
		gfs.close();

		ifstream kfs(ifile);
		kfs>>ixs;
		kfs.close();

		cout<<"x1s: "<<endl;
		cout<<x1s<<endl;
		cout<<"x2s: "<<endl;
		cout<<x2s<<endl;

		char* file1 = "data/robot/ix1s.txt";
		char* file2 = "data/robot/ix2s.txt";

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

		char* im1_file = "data/robot/1_small.jpg";
		char* im2_file = "data/robot/3_small.jpg";

		im1 = cvLoadImage(im1_file, 1);
		im2 = cvLoadImage(im2_file, 1);
	}


	void test() {
		vector<CvPoint3D32f> Xs;
		int i = 0;
		for (i = 3; i < 5; i++) {
			for (int j = -2; j < 3; j++) {
				Xs.push_back(cvPoint3D32f(j * 2, 0, i * 2));
			}
		}
		for (i = -1; i < 2; i++) {
			for (int j = 1; j < 3; j++) {
				Xs.push_back(cvPoint3D32f(i * 2, j, 6));
			}
		}

		//Camera cam1(1, cvPoint3D32f(0, 1.5, 0), cvPoint3D32f(0, 0, 1), cvPoint3D32f(0, 1, 0));
		//Camera cam2(1, cvPoint3D32f(0.3, 1.5, 1.2), cvPoint3D32f(0.2, 0, 1), cvPoint3D32f(0, 1, 0));
		double r1 = 10;
		double r2 = r1 + 5;
		double x = sin(3.1415/180*r1);
		double z = cos(3.1415/180*r1);
		cout<<"x: "<<x<<"  z: "<<z<<endl;
		Camera cam1(1, cvPoint3D32f(0, 1.5, 0), cvPoint3D32f(x, -0.2, z), cvPoint3D32f(0, 1, 0));
		double x1 = sin(3.1415/180*r2);
		double z1 = cos(3.1415/180*r2);
		cout<<"x1: "<<x1<<"  z1: "<<z1<<endl;
		Camera cam2(1, cvPoint3D32f(0.3, 1.5, 1.2), cvPoint3D32f(x1, -0.2, z1), cvPoint3D32f(0, 1, 0));
		
		TwoViewSimulation tvs(Xs, &cam1, &cam2, true, 0.001);
		//TwoViewSimulation tvs(Xs, &cam1, &cam2);

		vector<CvPoint2D32f> x1s = tvs.getx1s();
		vector<CvPoint2D32f> x2s = tvs.getx2s();

		IplImage* x1s_img = cvCreateImage(cvSize(400, 400), 8, 3);
		IplImage* x2s_img = cvCreateImage(cvSize(400, 400), 8, 3);

		vector<CvPoint2D32f> sx1s = scaleToWindow(x1s, 200);
		vector<CvPoint2D32f> sx2s = scaleToWindow(x2s, 200);

		::draw_correspondences_image("cos", x1s_img, cvPointsFrom32fs(sx1s), cvPointsFrom32fs(sx2s));

		vector<CvPoint2D32f> gx1s, gx2s, vx1s, vx2s;
		gx1s.insert(gx1s.begin(), x1s.begin(), x1s.end() - 6);
		vx1s.insert(vx1s.begin(), x1s.end() - 6, x1s.end());
		gx2s.insert(gx2s.begin(), x2s.begin(), x2s.end() - 6);
		vx2s.insert(vx2s.begin(), x2s.end() - 6, x2s.end());

		testVGHomography(vx1s, vx2s, gx1s, gx2s);

		CvMat* K = cam1.getK();
		cout<<"K: "<<K<<endl;

		DLT_HC hc;
		Homography vH = hc.compute(vx1s, vx2s);
		cout<<"v H: "<<vH.toString()<<endl;

		Homography gH = hc.compute(gx1s, gx2s);
		cout<<"g H: "<<gH.toString()<<endl;

		CvMat* nvH = matMul(inv(K), vH.getMat(), K);
		cout<<"nvH: "<<nvH<<endl;

		CvMat* ngH = matMul(inv(K), gH.getMat(), K);
		cvScale(ngH, ngH, 1 / cvmGet(ngH, 2, 2));

		cout<<"ngH: "<<ngH<<endl;


		CvMat* R1 = cam1.getR();
		cout<<"R1: "<<R1<<endl;

		CvMat* R2 = cam2.getR();
		cout<<"R2: "<<R2<<endl;

		CvMat* R21 = matMul(R2, inv(R1));
		cvScale(R21, R21, 1 / cvmGet(R21, 2, 2));

		cout<<"R21: "<<R21<<endl;

		CvMat* R12 = matMul(inv(R1), R2);
		cout<<"R12: "<<R12<<endl;

		//R1 = matMul(R1, R12);

		CvMat* nngH = matMul(inv(R1), ngH, R1);
		cvScale(nngH, nngH, 1 / cvmGet(nngH, 2, 2));
		cout<<"nngH: "<<nngH<<endl;

		for (size_t i = 0; i < Xs.size(); i++) {
			cout<<i<<" : "<<Xs[i]<<" -> "<<x1s[i]<<endl;
		}

		cout<<"R1: "<<R1<<endl;
		int a[] = {0, 1, 6, 5, 2, 3, 4};
		vector<CvPoint2D32f> ps;
		vector<CvPoint2D32f> ips;
		for (int i = 0; i < 5; i++) {
			ps.push_back(cvPoint2D32f(Xs[a[i]].x, Xs[a[i]].z));
			ips.push_back(x1s[a[i]]);
		}
		Homography H = hc.compute(ps, ips);
		cout<<"H: "<<H.getMat()<<endl;

		CvMat* cP = cvCreateMat(3, 3, CV_64FC1);
		setCol(cP, getCol(R1, 0), 0);
		setCol(cP, getCol(R1, 2), 1);
		setCol(cP, cam1.gett(), 2);
		cout<<"cP: "<<cP<<endl;
		CvMat* cH = matMul(cam1.getK(), cP);
		cout<<"cH: "<<cH<<endl;

		CvMat* HR = simpleEstimateR(H.getMat(), cam1.getK());
		cout<<"HR: "<<HR<<endl;
		cout<<"R1: "<<R1<<endl;

		vector<CvPoint2D32f> ps2;
		vector<CvPoint2D32f> ips2;
		int rc = 2;
		for (int i = 0; i < 5; i++) {
			//int index = a[(i + 0) % 4];
			int index = a[i];
			ps2.push_back(cvPoint2D32f(Xs[index].x - Xs[2].x, Xs[index].z - Xs[2].z));
			ips2.push_back(cvPoint2D32f(x1s[a[i]].x, x1s[a[i]].y));
		}
		cout<<"ps2: "<<ps2<<endl;
		cout<<"ips2: "<<ips2<<endl;
		Homography tH = hc.compute(ps2, ips2);
		cout<<"H: "<<tH.getMat()<<endl;
		
		CvMat* tHR = simpleEstimateR(tH.getMat(), cam1.getK());
		CvMat* ntHRH = normalizeH(ngH, tHR);
		cout<<"..1: "<<ntHRH<<endl;
		cout<<"nngh: "<<normalizeH(ngH, R1)<<endl;

		CvMat* M = matMul(cam1.getK(), HR);
		NormalizedHomographyBasedGPD gpd(x1s, x2s, M, M, 0.01);
		//gpd.fitModel(nngH);

		gpd.compute();
		cout<<"is: "<<endl;
		vector<int> is = gpd.getis();
		for (size_t i = 0; i < is.size(); i++) {
			cout<<is[i]<<endl;
		}


		//vector<int> is = gpd.getis();
		
		vector<CvPoint2D32f> ix1s, ix2s, ox1s, ox2s;
		for (size_t i = 0; i < is.size(); i++) {
			ix1s.push_back(x1s[is[i]]);
			ix2s.push_back(x2s[is[i]]);
		}
		vector<int> os = gpd.getos();
		for (size_t i = 0; i < os.size(); i++) {
			ox1s.push_back(x1s[os[i]]);
			ox2s.push_back(x2s[os[i]]);
		}

		vector<CvPoint2D32f> six1s = scaleToWindow(ix1s, 200);
		vector<CvPoint2D32f> sox1s = scaleToWindow(ox1s, 200);

		IplImage* img = cvCreateImage(cvSize(400, 400), 8, 3);
		cvZero(img);
		cvNamedWindow("s", 1);
		cvShowImage("s", img);
		drawGroundObstaclePointsImage("ground", img, cvPointsFrom32fs(six1s), cvPointsFrom32fs(sox1s));


/*
		Camera cam3(1, cvPoint3D32f(0, 1.5, 0), cvPoint3D32f(0, -0.2, 1), cvPoint3D32f(0.2, 1, 0.2));
		CvMat* R3 = cam3.getR();
		cout<<"R3: "<<R3<<endl;
		cout<<"inv R3: "<<inv(R3)<<endl;
		cout<<"trans R3: "<<transpose(R3)<<endl;
*/	}

	//H' = inv(M1) * H * M1 = dR * (I + tn)
	CvMat* normalizeH(CvMat* H, CvMat* M1) {
		return matMul(inv(M1), H, M1);
	}

	//M' = M ./ M(3, 3)
	CvMat* normalizeMat(CvMat* M) {
		CvMat* MT = cvCreateMat(3, 3, M->type);
		cvScale(M, MT, 1 / cvmGet(M, 2, 2));
		return MT;
	}

	//H = K[r1, r3, t]
	//r2 = r1 x r3
	CvMat* simpleEstimateR(CvMat* H, CvMat* K) {
		CvMat* invKH = matMul(inv(K), H);
		if (cvmGet(invKH, 0, 0) > 0) {
			cvScale(invKH, invKH, -1);
		}
		CvMat* r1 = getCol(invKH, 0);
		normalizeVector(r1);
		CvMat* r3 = getCol(invKH, 1);
		normalizeVector(r3);
		CvMat* r2 = cvCreateMat(3, 1, CV_64FC1);
		cvCrossProduct(r1, r3, r2);
		CvMat* R = cvCreateMat(3, 3, CV_64FC1);
		setCol(R, r1, 0);
		setCol(R, r2, 1);
		setCol(R, r3, 2);
		return R;
	}

	void testVGHomography(const vector<CvPoint2D32f>& vx1s, const vector<CvPoint2D32f>& vx2s, const vector<CvPoint2D32f>& gx1s, const vector<CvPoint2D32f>& gx2s) {

		DLT_HC hc;
		Homography vH = hc.compute(vx1s, vx2s);
		cout<<"v H: "<<vH.toString()<<endl;

		Homography gH = hc.compute(gx1s, gx2s);
		cout<<"g H: "<<gH.toString()<<endl;


		cout<<"v vH points errors ==================> "<<endl;
		for (int i = 0; i < (int)vx1s.size(); i++) {
			cout<<i<<": "<<vH.transfromDistance(vx1s[i], vx2s[i])<<endl;
		}

		cout<<"g vH points errors ==================>"<<endl;
		for (int i = 0; i < (int)gx1s.size(); i++) {
			cout<<i<<": "<<vH.transfromDistance(gx1s[i], gx2s[i])<<endl;
		}

		cout<<"v gH points errors ==================>"<<endl;
		for (int i = 0; i < (int)vx1s.size(); i++) {
			cout<<i<<": "<<gH.transfromDistance(vx1s[i], vx2s[i])<<endl;
		}

		cout<<"g gH points errors ==================>"<<endl;
		for (int i = 0; i < (int)gx1s.size(); i++) {
			cout<<i<<": "<<gH.transfromDistance(gx1s[i], gx2s[i])<<endl;
		}
	}

	vector<CvPoint2D32f> scaleToWindow(const vector<CvPoint2D32f>& xs, int halfWinSize) {
		vector<CvPoint2D32f> sxs;
		for (int i = 0; i < (int)xs.size(); i++) {
			sxs.push_back(scaleToWindow(xs[i], halfWinSize));
		}
		return sxs;
	}

	CvPoint2D32f scaleToWindow(const CvPoint2D32f& p, int halfWinSize) {
		return cvPoint2D32f(p.x * halfWinSize + halfWinSize, halfWinSize - p.y * halfWinSize); 
	}

};

