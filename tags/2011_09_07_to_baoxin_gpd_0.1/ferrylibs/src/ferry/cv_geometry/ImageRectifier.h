#pragma once

#include <fstream>

#include "highgui.h"
#include "cv.h"

#include "CvMatUtil.h"
#include "Camera.h"
#include "IOUtil.h"
#include "TwoViewGeometryUtil.h"
#include "BasicUtil.h"

using namespace std;
using namespace ferry::cv_mat;
using namespace ferry::cv_geometry::io_util;

namespace ferry {
	namespace cv_geometry {

class ImageRectifier
{
public:

	//Rx = o1 - o2
	//R = [Rx, Ry, Rz]'
	//K = {f, px, py}. px = width / 2, py = height / 2;
	//H1 = K * R * inv(R1) * inv(K1)
	//H2 = K * R * inv(R2) * inv(K2)
	//P1' = H1 * P1 = K * R * inv(R1) * inv(K1) * K1 * R1 * [I | -C1] = K * R * [I | -C1]
	static void estimateHomography(Camera* pcam1, Camera* pcam2, CvMat** H1, CvMat** H2) {

		CvPoint3D32f o1 = pcam1->geto();
		CvPoint3D32f o2 = pcam2->geto();

		//==DEBUG
		cout<<"o1: "<<o1.x<<", "<<o1.y<<", "<<o1.z<<endl;
		cout<<"o2: "<<o2.x<<", "<<o2.y<<", "<<o2.z<<endl;
		//==

		//CvPoint3D32f op = cvPoint3D32f(o2.x - o1.x, o2.y - o1.y, o2.z - o1.z);
		CvPoint3D32f op = cvPoint3D32f(o1.x - o2.x, o1.y - o2.y, o1.z - o2.z);
		CvMat* mop = matFromPoint3D(op);
		CvMat* K2 = pcam2->getK();
		CvMat* R2 = pcam2->getR();
		CvMat* tu = transpose(getRow(R2, 1)); //matFromPoint3D(cvPoint3D32f(0, 1, 0));
		CvMat* v = cvCreateMat(3, 1, CV_64FC1);
		cvCrossProduct(mop, tu, v);
		CvMat* u = cvCreateMat(3, 1, CV_64FC1);
		cvCrossProduct(v, mop, u);
		
		normalizeVector(mop);
		normalizeVector(u);
		normalizeVector(v);
		CvMat* Rt = cvCreateMat(3, 3, CV_64FC1);
		setSubRect(Rt, mop, cvRect(0, 0, 1, 3));
		setSubRect(Rt, u, cvRect(1, 0, 1, 3));
		setSubRect(Rt, v, cvRect(2, 0, 1, 3));
		CvMat* R = cvCreateMat(3, 3, CV_64FC1);
		cvT(Rt, R);
		
		//CvMat* R = Camera::calcR(matToPoint3D(v), matToPoint3D(u));

		//==DEBUG
		cout<<"mop: "<<matString(mop)<<endl;
		cout<<"v: "<<matString(v)<<endl;
		cout<<"u: "<<matString(u)<<endl;
		cout<<"R: "<<matString(R)<<endl;
		//==

		CvMat* K = cloneMat(K2);
		//cvmSet(K, 0, 2, 205);

		CvMat* K1 = pcam1->getK();
		CvMat* R1 = pcam1->getR();
		CvMat* K1t = cvCreateMat(3, 3, CV_64FC1);
		cvInvert(K1, K1t);
		CvMat* R1t = cvCreateMat(3, 3, CV_64FC1);
		cvInvert(R1, R1t);
		*H1 = matMul(K, matMul(R, R1t), K1t);

		//==DEBUG
		cout<<"K1: "<<K1<<endl;
		cout<<"K1t: "<<K1t<<endl;
		cout<<"K1t * K1: "<<matMul(K1t, K1)<<endl;
		cout<<"R1: "<<R1<<endl;
		cout<<"R1t: "<<R1t<<endl;
		cout<<"R1t * R1: "<<matMul(R1t, R1)<<endl;
		cout<<"R * R1t: "<<matMul(R, R1t)<<endl;
		cout<<"H1: "<<*H1<<endl;
		//==

		CvMat* K2t = cvCreateMat(3, 3, CV_64FC1);
		cvInvert(pcam2->getK(), K2t);
		CvMat* R2t = cvCreateMat(3, 3, CV_64FC1);
		cvInvert(pcam2->getR(), R2t);
		cvmSet(K, 0, 2, cvmGet(K, 0, 2));
		*H2 = matMul(K, matMul(R, R2t), K2t);

		//==DEBUG
		cout<<"K2: "<<K2<<endl;
		cout<<"R2: "<<R2<<endl;
		CvMat* M2 = matMul(K2, R2);
		cout<<"M2: "<<M2<<endl;

		cout<<"K: "<<K<<endl;
		cout<<"R: "<<R<<endl;
		CvMat* M0 = matMul(K, R);
		cout<<"M0: "<<M0<<endl;

		cout<<"K1: "<<K1<<endl;
		cout<<"R1: "<<R1<<endl;
		CvMat* M1 = matMul(K1, R1);
		cout<<"M1: "<<M1<<endl;

		cout<<"inv(H2) * H1: "<<matMul(inv(*H2), *H1)<<endl;
		cout<<"M2 * inv(M1): "<<matMul(M2, inv(M1))<<endl;

		cout<<"H2: "<<*H2<<endl;
		//==
	}

	static void simpleEstimateHomography(Camera* pcam1, Camera* pcam2, CvMat** H1, CvMat** H2) {
		CvMat* me1 = matMul(pcam1->getP(), pcam2->getHC());
		CvMat* me2 = matMul(pcam2->getP(), pcam1->getHC());

		CvPoint2D32f e1 = hmatToPoint2D(me1);
		CvPoint2D32f e2 = hmatToPoint2D(me2);

		*H1 = getEH(e1);
		*H2 = getEH(e2);
	}

	//H' = GRT
	//T = [1, 0, -w/2; 0, 1, -h/2; 0, 0, 1]
	//R = [cos(theta), -sin(theta), 0; sin(theta), cos(theta), 0; 0, 0, 1]; theta = - arctan((ey - h/2)/(ex - w/2))
	//G = [1, 0, 0; 0, 1, 0; -1/f, 0, 1]; 
	//H = Ha H0 = Ha H' M; F = [e']xM
	static void hartleyEstimationBasedOnF(CvMat* F, const CvSize& im_size, CvMat** H1, CvMat** H2) {
		CvMat* e2 = leftNull(F);
		
		CvPoint2D32f ep = hmatToPoint2D(e2);

		double tx = - (double)im_size.width / 2;
		double ty = - (double)im_size.height / 2;

		double ex1 = ep.x + tx;
		double ey1 = ep.y + ty;

		double theta = - atan(ey1 / ex1);

		double ex2 = cos(theta) * ex1 - sin(theta) * ey1;
		double ey2 = sin(theta) * ex1 + cos(theta) * ey1;

		double t[] = {1, 0, tx, 0, 1, ty, 0, 0, 1};
		double r[] = {cos(theta), -sin(theta), 0, sin(theta), cos(theta), 0, 0, 0, 1};
		double g[] = {1, 0, 0,  0, 1, 0,  -1/ex2, 0, 1};

		double nt[] = {1, 0, -tx, 0, 1, -ty, 0, 0, 1};

		CvMat T = cvMat(3, 3, CV_64FC1, t);
		CvMat R = cvMat(3, 3, CV_64FC1, r);
		CvMat G = cvMat(3, 3, CV_64FC1, g);

		CvMat NT = cvMat(3, 3, CV_64FC1, nt);

		CvMat* oH2 = matMul(&G, &R, &T);
		*H2 = matMul(&NT, oH2);


		CvMat* e2x = getCrossMatrix(e2);
		//CvMat* inv_e2x = cvCreateMat(3, 3, CV_64FC1);
		//cvInvert(e2x, inv_e2x, CV_SVD);
		CvMat* e2xF = matMul(e2x, F);

		CvMat* I = cvCreateMat(3, 3, CV_64FC1);
		cvSetIdentity(I);
		double v[] = {0.08, 0, 0};
		CvMat V = cvMat(3, 1, CV_64FC1, v);
		CvMat* e2Vt = matMul(e2, transpose(&V));

		CvMat* M = cvCreateMat(3, 3, CV_64FC1);
		cvAdd(e2xF, e2Vt, M);

		//==DEBUG
		cout<<"F: "<<F<<endl;
		cout<<"[e2]xM: "<<matMul(e2x, M)<<endl;
		cout<<"e2xe2xF: "<<matMul(e2x, e2x, F)<<endl;
		//==

		CvMat* oH1 = matMul(oH2, M);
		//*H1 = matMul(&NT, oH1);
		*H1 = matMul(*H2, M);

		//==DEBUG
		cout<<"M: "<<M<<endl;
		cout<<"det(M): "<<cvDet(M)<<endl;
		cout<<"det(F): "<<cvDet(F)<<endl;
		cout<<"e2: "<<e2<<endl;
		cout<<"e2x: "<<e2x<<endl;
		CvMat* e1 = null(F);
		cout<<"e1: "<<e1<<endl;
		cout<<"H1e1: "<<matMul(*H1, e1)<<endl;
		cout<<"H2e2: "<<matMul(*H2, e2)<<endl;
		cout<<"e2: "<<e2<<endl;
		cout<<"Me1: "<<matMul(M, e1)<<endl;
		cout<<"H1: "<<*H1<<endl;
		cout<<"H2: "<<*H2<<endl;

		CvMat* oH10 = matMul(oH2, e2xF);
		cout<<"oH10: "<<oH10<<endl;
		CvMat* inv_oH1 = cvCreateMat(3, 3, CV_64FC1);
		cvInvert(oH10, inv_oH1, CV_SVD);
		cout<<"inv(oH10): "<<inv_oH1<<endl;
		cout<<"inv(oH10) * oH10: "<<matMul(inv_oH1, oH10)<<endl;
		cout<<"oH10 * inv(oH10): "<<matMul(oH10, inv_oH1)<<endl;
		CvMat* vtinv_oH1 = matMul(transpose(&V), inv_oH1);
		cout<<"vt * inv(oH10): "<<vtinv_oH1<<endl;
		CvMat* oH2e2 = matMul(oH2, e2);
		cout<<"oH2 * e2: "<<oH2e2<<endl;
		CvMat* AT = matMul(oH2e2, vtinv_oH1);
		cout<<"oH2 * e2 * vt * inv_oH1: "<<AT<<endl;
		CvMat* IpT = cvCreateMat(3, 3, CV_64FC1);
		cvAdd(I, AT, IpT);
		cout<<"IpT: "<<IpT<<endl;
		CvMat* IpToH10 = matMul(IpT, oH10);
		cout<<"IpToH10: "<<IpToH10<<endl;
		cout<<"oH1: "<<oH1<<endl;
		//==
	}

	//H2 = K0 * R0 * inv(R2) * inv(K2)
	//R2 = I
	//K0 = K2 = [f, 0, px; 0, f, py; 0, 0, 1]; f = (width + height)/2, px = -width/2, py = -height/2
	//R0 = [r, s, t]'
	//r = |inv(K2) * e2| and r(0) > 0
	//s2 = [0, 1, 0]'
	//t = r x s2
	//s = t x r
	//
	static void similarityEstimationBasedOnF(CvMat* F, const CvSize& im_size, CvMat** H1, CvMat** H2) {
		double f = (double)(im_size.width + im_size.height)/5;
		double px = -(double)im_size.width / 2;
		double py = -(double)im_size.height / 2;
		double k2[] = {f, 0, px, 0, f, py, 0, 0, 1};
		CvMat  sK2 = cvMat(3, 3, CV_64FC1, k2);
		CvMat* K2 = &sK2;
		CvMat* K0 = &sK2;
		CvMat* inv_K2 = cvCreateMat(3, 3, CV_64FC1);
		cvInvert(K2, inv_K2);
		
		CvMat* e2 = leftNull(F);
		CvMat* r = matMul(inv_K2, e2);
		normalizeVector(r);
		if (cvmGet(r, 0, 0) < 0) {
			cvScale(r, r, -1.0);
		}
		CvMat* s2 = matFromPoint3D(cvPoint3D32f(0, 1, 0));
		CvMat* t = cvCreateMat(3, 1, CV_64FC1);
		cvCrossProduct(r, s2, t);
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

		*H2 = matMul(K0, R0, inv_K2);
		*H1 = getIdentity();
	}

private:
	static CvMat* getEH(const CvPoint2D32f& e) {
		CvMat* H = cvCreateMat(3, 3, CV_64FC1);
		cvSetIdentity(H);
		cvmSet(H, 1, 0, -e.y / e.x);
		cvmSet(H, 2, 0, -1 / e.x);
		return H;
	}

	static CvMat* getK(double f, double width, double height) {
		double px = width / 2;
		double py = height / 2;
		CvMat* K = cvCreateMat(3, 3, CV_64FC1);
		cvZero(K);
		cvmSet(K, 0, 0, f);
		cvmSet(K, 1, 1, f);
		cvmSet(K, 0, 2, px);
		cvmSet(K, 1, 2, py);
		cvmSet(K, 2, 2, 1);
		return K;
	}

};

class PointsBasedImageRectifier {
public:
	virtual void compute(CvMat* F, const CvSize& im_size, const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s, CvMat** H1, CvMat** H2) = 0;
};

class Hartley_PBIR: public PointsBasedImageRectifier{
public:
	void compute(CvMat* F, const CvSize& im_size, const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s, CvMat** H1, CvMat** H2) {
		*H2 = estimateH2(F, im_size);
		*H1 = estimateH1(F, *H2, x1s, x2s);
	}

public:
	static CvMat* estimateH2(CvMat* F, const CvSize& im_size) {
		CvMat* e2 = leftNull(F);
		
		CvPoint2D32f ep = hmatToPoint2D(e2);

		double tx = - (double)im_size.width / 2;
		double ty = - (double)im_size.height / 2;

		double ex1 = ep.x + tx;
		double ey1 = ep.y + ty;

		double theta = - atan(ey1 / ex1);

		double ex2 = cos(theta) * ex1 - sin(theta) * ey1;
		double ey2 = sin(theta) * ex1 + cos(theta) * ey1;

		double t[] = {1, 0, tx, 0, 1, ty, 0, 0, 1};
		double r[] = {cos(theta), -sin(theta), 0, sin(theta), cos(theta), 0, 0, 0, 1};
		double g[] = {1, 0, 0,  0, 1, 0,  -1/ex2, 0, 1};

		double nt[] = {1, 0, -tx, 0, 1, -ty, 0, 0, 1};

		CvMat T = cvMat(3, 3, CV_64FC1, t);
		CvMat R = cvMat(3, 3, CV_64FC1, r);
		CvMat G = cvMat(3, 3, CV_64FC1, g);

		CvMat NT = cvMat(3, 3, CV_64FC1, nt);

		CvMat* oH2 = matMul(&G, &R, &T);
		CvMat* H2 = matMul(&NT, oH2);
		return H2;
	}

	//F = [e2]xM
	//H1 = Ha * H2 * M = Ha * H0
	CvMat* estimateH1(CvMat* F, CvMat* H2, const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s) {
		CvMat* M = getFM(F, 0.2);
		CvMat* H0 = matMul(H2, M);

		vector<CvPoint2D32f> tx1s = transformPoints(H0, x1s);
		vector<CvPoint2D32f> tx2s = transformPoints(H2, x2s);

		double a, b, c;
		compute_abc(tx1s, tx2s, a, b, c);

		CvMat* Ha = getIdentity();
		cvmSet(Ha, 0, 0, a);
		cvmSet(Ha, 0, 1, b);
		cvmSet(Ha, 0, 2, c);

		CvMat* H1 = matMul(Ha, H0);

		//==DEBUG
		cout<<"a b c: "<<a<<" "<<b<<" "<<c<<endl;
		cout<<"H0: "<<H0<<endl;
		cout<<"Ha: "<<Ha<<endl;
		cout<<"H1: "<<H1<<endl;
		vector<CvPoint2D32f> atx1s = transformPoints(Ha, tx1s);
		cout<<"atx1s: "<<atx1s<<endl;
		cout<<"tx2s: "<<tx2s<<endl;
		//==

		cvReleaseMat(&Ha);
		return H1;
	}

	void compute_abc(const vector<CvPoint2D32f>& tx1s, const vector<CvPoint2D32f>& tx2s, double& a, double& b, double& c) {
		int n = tx1s.size();
		double* A = new double[n * 3];
		double* C = new double[n];

		for (int i = 0; i < n; i++) {
			A[i * 3] = tx1s[i].x;
			A[i * 3 + 1] = tx1s[i].y;
			A[i * 3 + 2] = 1;
			C[i] = tx2s[i].x;
		}

		CvMat AM = cvMat(n, 3, CV_64FC1, A);
		CvMat CM = cvMat(n, 1, CV_64FC1, C);
		CvMat* BM = cvCreateMat(3, 1, CV_64FC1);
		cvSolve(&AM, &CM, BM, CV_SVD);

		//==DEBUG
		cout<<"AM: "<<&AM<<endl;
		cout<<"CM: "<<&CM<<endl;
		cout<<"BM: "<<BM<<endl;
		cout<<"AM * BM: "<<matMul(&AM, BM)<<endl;
		cout<<"AM * BM - CM: "<<sub(matMul(&AM, BM), &CM)<<endl;
		//==

		a = cvmGet(BM, 0, 0);
		b = cvmGet(BM, 1, 0);
		c = cvmGet(BM, 2, 0);
		cvReleaseMat(&BM);

	}
};

class TrueStereo_PBIR : public PointsBasedImageRectifier{
public:
	TrueStereo_PBIR() {
		alpha = 0.25;
		x0disparity = 0;
		p2xratio = 0;
		p2yratio = 0;
		fyratio = 0;
	}

	void setAlpha(double a) {
		alpha = a;
	}

	void setX0Disparity(double d) {
		x0disparity = d;
	}

	void setPrincipleXRatio(double xr) {
		p2xratio = xr;
	}

	void setPrincipleYRatio(double yr) {
		p2yratio = yr;
	}

	void setFYRatio(double r) {
		fyratio = r;
	}

public:
	void compute(CvMat* F, const CvSize& im_size, const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s, CvMat** H1, CvMat** H2) {
		*H2 = estimateH2(F, im_size);
		*H1 = estimateH1(F, *H2, x1s, x2s);
/*		
		vector<CvPoint2D32f> tx1s, tx2s;
		tx1s = transformPoints(H1, x1s);
		tx2s = transformPoints(H2, x2s);
		int minIndex;
		double minDistance = tx1s[0].x - tx2s[0].x;
		for (int i = 0; i < tx1s.size(); i++) {
			cout<<i<<" : "<<tx1s[i].x - tx2s[i].x<<", "<<tx1s[i].y - tx2s[i].y<<endl;
		}
*/		
		cout<<"inv(H2) * H1: "<<matMul(inv(*H2), *H1)<<endl;
	}

private:
	CvMat* estimateH2(CvMat* F, const CvSize& im_size) {
		double f = im_size.width * alpha; //(double)(im_size.width + im_size.height)/8;
		double px = (double)im_size.width / 2 * (1 + p2xratio);
		double py = (double)im_size.height / 2 * (1 + p2yratio);
		//double k2[] = {f, 0, px, 0, f, py, 0, 0, 1};
		double k2[] = {f, 0, px, 0, -f * (1 + fyratio), py, 0, 0, 1};
		CvMat  sK2 = cvMat(3, 3, CV_64FC1, k2);
		CvMat* K2 = &sK2;
		K0 = cloneMat(&sK2);
		CvMat* inv_K2 = cvCreateMat(3, 3, CV_64FC1);
		cvInvert(K2, inv_K2);
		
		CvMat* e2 = leftNull(F);
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
		R0 = cvCreateMat(3, 3, CV_64FC1);
		cvT(R0t, R0);

		//==DEBUG
		cout<<"R0: "<<R0<<endl;
		//==

		CvMat* H2 = matMul(K0, R0, inv_K2);

		//==DEBUG
		cout<<"K2: "<<K2<<endl;
		cout<<"K0: "<<K0<<endl;
		cout<<"R0: "<<R0<<endl;
		cout<<"H2: "<<H2<<endl;
		cout<<"inv_K2: "<<inv_K2<<endl;
		cout<<"K0 * inv_K2: "<<matMul(K0, inv_K2)<<endl;

		ofstream ofs("eK.txt");
		ofs<<H2<<endl;
		ofs.close();

		ofstream eofs("e.txt");
		eofs<<e2<<endl;
		eofs.close();

		//==

		return H2;
	}

	//F = [e2]xM
	//H1 = Ha * H2 * M = Ha * H0
	CvMat* estimateH1(CvMat* F, CvMat* H2, const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s) {
		CvMat* M = getFM(F, 1);
		CvMat* H0 = matMul(H2, M);

		vector<CvPoint2D32f> tx1s = transformPoints(H0, x1s);
		vector<CvPoint2D32f> tx2s = transformPoints(H2, x2s);

		double a, b, c;
		compute_abc(x1s, x2s, tx1s, tx2s, a, b, c);

		CvMat* Ha = getIdentity();
		cvmSet(Ha, 0, 0, a);
		cvmSet(Ha, 0, 1, b);
		cvmSet(Ha, 0, 2, c);

		H1 = matMul(Ha, H0);

		//==DEBUG
		cout<<"a b c: "<<a<<" "<<b<<" "<<c<<endl;
		cout<<"H0: "<<H0<<endl;
		cout<<"Ha: "<<Ha<<endl;
		cout<<"H1: "<<H1<<endl;
		//vector<CvPoint2D32f> atx1s = transformPoints(Ha, tx1s);
		//cout<<"atx1s: "<<atx1s<<endl;
		//cout<<"tx2s: "<<tx2s<<endl;
		//==

		cvReleaseMat(&Ha);
		return H1;
	}

	void compute_abc(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s, const vector<CvPoint2D32f>& tx1s, const vector<CvPoint2D32f>& tx2s, double& a, double& b, double& c, int min_cluster_size = 2, double max_d = 2.2) {
		const int MAX_ROW_NUM = 200;
		int row_num = 0;
		double A[MAX_ROW_NUM * 2];
		double C[MAX_ROW_NUM];
		bool stop = false;//int t = 0;

		vector< vector<int> > clusters = getSamePlanePoints(x1s, x2s, max_d);
		for (int i = 0; i < (int)clusters.size(); i++) {
			if (clusters[i].size() < min_cluster_size) continue;
			for (int j = 0; j < (int)clusters[i].size() - 1; j++) {
				int index_i = clusters[i][j];
				for (int k = j + 1; k < (int)clusters[i].size(); k++) {
					int index_j = clusters[i][k];
					//==DEBUG
/*					cout<<"index_i: "<<index_i<<endl;
					cout<<"index_j: "<<index_j<<endl;
					cout<<"x1_i: "<<x1s[index_i]<<endl;
					cout<<"x1_j: "<<x1s[index_j]<<endl;
					cout<<"x2_i: "<<x2s[index_i]<<endl;
					cout<<"x2_j: "<<x2s[index_j]<<endl;
					cout<<"tx1_i: "<<tx1s[index_i]<<endl;
					cout<<"tx1_j: "<<tx1s[index_j]<<endl;
					cout<<"tx2_i: "<<tx2s[index_i]<<endl;
					cout<<"tx2_j: "<<tx2s[index_j]<<endl;
*/					//==
					A[row_num * 2] = tx1s[index_i].x - tx1s[index_j].x;
					A[row_num * 2 + 1] = tx1s[index_i].y - tx1s[index_j].y;
					C[row_num] = tx2s[index_i].x - tx2s[index_j].x;
					row_num++;
					if (row_num >= MAX_ROW_NUM) {
						stop = true;
						break;
					}
				}
				if (stop) break;
			}
			if (stop) break;
		}
		CvMat AM = cvMat(row_num, 2, CV_64FC1, A);
		CvMat CM = cvMat(row_num, 1, CV_64FC1, C);
		CvMat* BM = cvCreateMat(2, 1, CV_64FC1);
		cvSolve(&AM, &CM, BM, CV_SVD);

		//==DEBUG
/*		cout<<"AM: "<<&AM<<endl;
		cout<<"CM: "<<&CM<<endl;
		cout<<"BM: "<<BM<<endl;
		cout<<"AM * BM: "<<matMul(&AM, BM)<<endl;
		cout<<"AM * BM - CM: "<<sub(matMul(&AM, BM), &CM)<<endl;
*/		//==

		a = cvmGet(BM, 0, 0);
		b = cvmGet(BM, 1, 0);
		c = tx2s[0].x + x0disparity - (a * tx1s[0].x + b * tx1s[0].y);
		cvReleaseMat(&BM);
	}

	vector<vector<int> > getSamePlanePoints(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s, double max_d) {
		vector<CvPoint2D32f> disparities = getDisparities(x1s, x2s);
		vector<CvPoint2D32f> centers;
		vector< vector <int> > clusters;
		for (int i = 0; i < (int)disparities.size(); i++) {
			CvPoint2D32f p = disparities[i];
			bool newc = true;
			for (int j = 0; j < centers.size(); j++) {
				if (pointDistance(p, centers[j]) <= max_d) {
					centers[j].x = (centers[j].x * clusters[j].size() + p.x) / (clusters[j].size() + 1);
					centers[j].y = (centers[j].y * clusters[j].size() + p.y) / (clusters[j].size() + 1);
					clusters[j].push_back(i);
					newc = false;
					break;
				}
			}
			if (newc) {
				centers.push_back(p);
				vector<int> cluster;
				cluster.push_back(i);
				clusters.push_back(cluster);
			}
		}

		//==DEBUG

		int maxsize = 0;
		int maxi = 0;
		for (int i = 0; i < (int)clusters.size(); i++) {
			cout<<"cluster "<<i<<" "<<centers[i]<<" ........."<<endl;
			if (clusters[i].size() > maxsize) {
				maxsize = clusters[i].size();
				maxi = i;
			}
			for (int j = 0; j < (int)clusters[i].size(); j++) {
				cout<<clusters[i][j]<<endl;
			}
		}
		ofstream ofs("cluster.txt");
		ofs<<clusters[maxi].size()<<endl;
		for (int i = 0; i < clusters[maxi].size(); i++) {
			ofs<<clusters[maxi][i]<<endl;
		}
		ofs<<max_d<<endl;
		ofs<<centers[maxi]<<endl;
		for (int i = 0; i < clusters[maxi].size(); i++) {
			ofs<<disparities[clusters[maxi][i]]<<endl;
		}
		ofs.close();

		char file[200];
		for (int i = 0; i < (int)clusters.size(); i++) {
			sprintf(file, "cluster%d.txt", i);
			ofstream ofs(file);
			ofs<<clusters[i].size()<<endl;
			for (int j = 0; j < clusters[i].size(); j++) {
				ofs<<clusters[i][j]<<endl;
			}
			ofs.close();
		}

		
		//==


		return clusters;
	}

	vector<CvPoint2D32f> getDisparities(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s) {
		vector<CvPoint2D32f> disparities;
		for (int i = 0; i < (int)x1s.size(); i++) {
			disparities.push_back(cvPoint2D32f(x1s[i].x - x2s[i].x, x1s[i].y - x2s[i].y));
		}

		//==DEBUG
		//cout<<"x1s: "<<endl;
		//cout<<x1s<<endl;
		//cout<<"x2s: "<<endl;
		//cout<<x2s<<endl;
		for (int i = 0; i < (int)disparities.size(); i++) {
			cout<<i<<": "<<disparities[i]<<endl;
		}
		//==
		return disparities;
	}

public:
	CvMat* getK0() {
		return K0;
	}

	CvMat* getR0() {
		return R0;
	}

	CvMat* getH1() {
		return H1;
	}

private:
	int x0disparity;
	double alpha;//ForFocalLength;
	double p2xratio; //principle point x
	double p2yratio; //principle point y
	double fyratio;

private:
	CvMat* K0;
	CvMat* R0;
	CvMat* H1;
};


	}
}