#pragma once

#include <vector>

#include "highgui.h"
#include "cv.h"

#include "CvMatUtil.h"

using namespace std;
using namespace ferry::cv_mat;

namespace ferry {
	namespace cv_geometry {

class Camera
{
public:
	//P = K[R|t], t = -RC
	Camera(double f, const CvPoint3D32f& origin, const CvPoint3D32f& viewDirection, const CvPoint3D32f& upDirection) {

		R = calcR(viewDirection, upDirection);
		o = origin;
		C = CvMatUtil::matFromPoint3D(origin);
		HC = CvMatUtil::hmatFromMat(C);
		t = cvCreateMat(3, 1, CV_64FC1);
		cvMatMul(R, C, t);
		cvScale(t, t, -1);

		//cout<<"R "<<CvMatUtil::matString(R)<<endl;
		//cout<<"C "<<CvMatUtil::matString(C)<<endl;
		//cout<<"t "<<CvMatUtil::matString(t)<<endl;

		NP = cvCreateMat(3, 4, CV_64FC1);
		CvMatUtil::setSubRect(NP, R, cvRect(0, 0, 3, 3));
		CvMatUtil::setCol(NP, t, 3);
		//cout<<"[R|t]: \r\n"<<CvMatUtil::matString(NP)<<endl;
		
		K = createCalibrationMatrix(f);
		//cout<<"K "<<K<<endl;
		
		P = cvCreateMat(3, 4, CV_64FC1);
		cvMatMul(K, NP, P);
		cout<<"Camera Matrix: \r\n"<<CvMatUtil::matString(P)<<endl;
	}

	Camera(CvMat* K, CvMat* NP) {
		this->K = K;
		this->NP = NP;
		
		P = cvCreateMat(3, 4, CV_64FC1);
		cvMatMul(K, NP, P);
		
		R = cvCreateMat(3, 3, CV_64FC1);
		CvMatUtil::copyCols(NP, R, 0, 3);
		
		t = cvCreateMat(3, 1, CV_64FC1);
		CvMatUtil::copyCol(NP, t, 3);

		HC = CvMatUtil::null(P);
		C = CvMatUtil::hmatToMat(HC);
		o = CvMatUtil::hmatToPoint3D(HC);
	}

	Camera(CvMat* K, CvMat* R, CvMat* t) {
		this->K = K;
		NP = cvCreateMat(3, 4, CV_64FC1);
		setCols(NP, R, 0, 3);
		setCol(NP, t, 3);

		this->R = R;
		this->t = t;

		P = matMul(K, NP);

		HC = null(P);
		C = hmatToMat(HC);
		o = hmatToPoint3D(HC);
	}

public:
	CvPoint2D32f calcImagePoint(CvPoint3D32f X) {
		return calcImagePoint(P, X);
	}

	vector<CvPoint2D32f> calcImagePoints(const vector<CvPoint3D32f>& Xs) {
		return calcImagePoints(P, Xs);
	}

	CvPoint2D32f calcImagePoint(CvMat* mX) {
		return calcImagePoint(P, mX);
	}

	CvPoint3D32f calcPoint3DWithDepth(CvPoint2D32f p, double depth) {
		return calcPoint3DWithDepth(P, p, depth);
	}

	//calc point coordinates in camera coordinates system
	//Xs: points in world coordinates system
	//out: points in camera coordinates system
	vector<CvPoint3D32f> calcCoordPoints(const vector<CvPoint3D32f>& Xs) {
		vector<CvPoint3D32f> nps;
		for (int i = 0; i < (int)Xs.size(); i++) {
			nps.push_back(calcCoordPoint(Xs[i]));
		}
		return nps;
	}

	CvPoint3D32f calcCoordPoint(const CvPoint3D32f& X) {
		CvMat* mX = CvMatUtil::hmatFromPoint3D(X);
		CvMat* mnX = CvMatUtil::matMul(NP, mX);
		CvPoint3D32f p = cvPoint3D32f(cvmGet(mnX, 0, 0), cvmGet(mnX, 1, 0), cvmGet(mnX, 2, 0));
		cvReleaseMat(&mX);
		cvReleaseMat(&mnX);
		return p;
	}

public:

	static vector<CvPoint2D32f> calcImagePoints(const CvMat* P, const vector<CvPoint3D32f>& Xs) {
		vector<CvPoint2D32f> xs;
		for (int i = 0; i < (int)Xs.size(); i++) {
			xs.push_back(calcImagePoint(P, Xs[i]));
		}
		return xs;
	}

	static CvPoint2D32f calcImagePoint(const CvMat* P, CvPoint3D32f X) {
		CvMat* mX = CvMatUtil::hmatFromPoint3D(X);
		CvPoint2D32f p = calcImagePoint(P, mX);
		cvReleaseMat(&mX);
		return p;
	}

	//mX: homogeneous
	static CvPoint2D32f calcImagePoint(const CvMat* P, const CvMat* mX) {
		CvMat* mp = cvCreateMat(3, 1, CV_64FC1);
		cvMatMul(P, mX, mp);
		CvPoint2D32f p = CvMatUtil::hmatToPoint2D(mp);
		cvReleaseMat(&mp);
		return p;
	}

	//p is in image coordinate system. in which the (0, 0) is the left-bottom point
	static CvPoint3D32f calcPoint3DWithDepth(const CvMat* P, CvPoint2D32f p, double depth) {
		double c0, c1, c2;
		double x, y, z = depth;
		double u = p.x;
		double v = p.y;

		c0 = z * cvmGet(P, 0, 2) + cvmGet(P, 0, 3);
		c1 = z * cvmGet(P, 1, 2) + cvmGet(P, 1, 3);
		c2 = z * cvmGet(P, 2, 2) + cvmGet(P, 2, 3);

		y = u * (c1 * cvmGet(P, 2, 0) - cvmGet(P, 1, 0) * c2) +
			v * (c2 * cvmGet(P, 0, 0) - cvmGet(P, 2, 0) * c0) +
			cvmGet(P, 1, 0) * c0 - c1 * cvmGet(P, 0, 0);

		y /= v * (cvmGet(P, 2, 0) * cvmGet(P, 0, 1) - cvmGet(P, 2, 1) * cvmGet(P, 0, 0)) +
			 u * (cvmGet(P, 1, 0) * cvmGet(P, 2, 1) - cvmGet(P, 1, 1) * cvmGet(P, 2, 0)) +
			cvmGet(P, 0, 0) * cvmGet(P, 1, 1) - cvmGet(P, 1, 0) * cvmGet(P, 0, 1);
			
		x = y * (cvmGet(P, 0, 1) - cvmGet(P, 2, 1) * u) + c0 - c2 * u;
		x /= cvmGet(P, 2, 0) * u - cvmGet(P, 0, 0);

		return cvPoint3D32f(x, y, z);
	}

	//P = H * P
	//K = H * K
	void trans(CvMat* H) {
		P = matMul(H, P); //memory leak!
		K = matMul(H, K);
	}

	void transWithScale(double scale) {
		double mh[] = {scale, 0, 0, 0, scale, 0, 0, 0, 1};
		CvMat MH = cvMat(3, 3, CV_64FC1, mh);
		trans(&MH);
	}

	void transWithFlipY(double h) {
		double mh[] = {1, 0, 0, 0, -1, h - 1, 0, 0, 1};
		CvMat MH = cvMat(3, 3, CV_64FC1, mh);
		trans(&MH);
	}

public:
	CvMat* getP() {
		return P;
	}

	CvMat* getK() {
		return K;
	}

	CvMat* getNP() {
		return NP;  //memory leack!
	}

	CvMat* getC() {
		return C;
	}

	CvMat* getHC() {
		return HC;
	}

	CvMat* getR() {
		return R;
	}

	CvMat* gett() {
		return t;
	}

	CvPoint3D32f geto() {
		return o;
	}

public:

	// l = l / ||l||;  r = (l x u) / ||l x u||, s = r x l
	// R = [r, s, l]'
	static CvMat* calcR(const CvPoint3D32f& viewDirection, const CvPoint3D32f& upDirection) {

		CvMat* l = CvMatUtil::matFromPoint3D(viewDirection);
		CvMat* u = CvMatUtil::matFromPoint3D(upDirection);

		//cout<<"l "<<CvMatUtil::matString(l)<<endl;
		//cout<<"u "<<CvMatUtil::matString(u)<<endl;

		CvMatUtil::normalizeVector(l);

		CvMat* r = cvCreateMat(3, 1, CV_64FC1);
		cvCrossProduct(l, u, r);
		CvMatUtil::normalizeVector(r);
		
		CvMat* s = cvCreateMat(3, 1, CV_64FC1);
		cvCrossProduct(r, l, s);

		//cout<<"r "<<CvMatUtil::matString(r)<<endl;
		//cout<<"s "<<CvMatUtil::matString(s)<<endl;
		//cout<<"l "<<CvMatUtil::matString(l)<<endl;

		CvMat* Rt = cvCreateMat(3, 3, CV_64FC1);
		CvMatUtil::setCol(Rt, r, 0);
		CvMatUtil::setCol(Rt, s, 1);
		CvMatUtil::setCol(Rt, l, 2);

		CvMat* R = cvCreateMat(3, 3, CV_64FC1);
		cvTranspose(Rt, R);

		cvReleaseMat(&l);
		cvReleaseMat(&u);
		cvReleaseMat(&r);
		cvReleaseMat(&s);
		cvReleaseMat(&Rt);

		return R;
	}

public:
	static CvMat* createCalibrationMatrix(double f) {
		CvMat* K = cvCreateMat(3, 3, CV_64FC1);
		cvZero(K);
		cvmSet(K, 0, 0, f);
		cvmSet(K, 1, 1, f);
		cvmSet(K, 2, 2, 1.0);
		return K;
	}

private:
	CvMat* P;
	CvMat* NP; //normalized camera matrix. NP = [R|t]

	CvMat* K;
	CvMat* R;
	CvMat* C;
	CvMat* t;

	//C in homogeneous coordinates
	CvMat* HC;

	CvPoint3D32f o;
};

	}
}