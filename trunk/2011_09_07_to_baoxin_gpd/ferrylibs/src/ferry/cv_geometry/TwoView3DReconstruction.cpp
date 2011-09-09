#include ".\twoview3dreconstruction.h"

namespace ferry {
	namespace cv_geometry {

CvMat* triangulate(const CvMat* P1, const CvMat* P2, const CvPoint2D32f& x1, const CvPoint2D32f& x2) {
	
	CvMat* A = cvCreateMat(4, 4, CV_64FC1);
	
	CvMat* p1t = getRow(P1, 0);
	CvMat* p2t = getRow(P1, 1);
	CvMat* p3t = getRow(P1, 2);
	CvMat* xp3t = scale(p3t, x1.x);
	CvMat* a1 = sub(xp3t, p1t);
	CvMat* yp3t = scale(p3t, x1.y);
	CvMat* a2 = sub(yp3t, p2t);

	CvMat* p1t2 = getRow(P2, 0);
	CvMat* p2t2 = getRow(P2, 1);
	CvMat* p3t2 = getRow(P2, 2);
	CvMat* xp3t2 = scale(p3t2, x2.x);
	CvMat* a3 = sub(xp3t2, p1t2);
	CvMat* yp3t2 = scale(p3t2, x2.y);
	CvMat* a4 = sub(yp3t2, p2t2);

	setRow(A, a1, 0);
	setRow(A, a2, 1);
	setRow(A, a3, 2);
	setRow(A, a4, 3);
	
	cvReleaseMat(&p1t);
	cvReleaseMat(&p2t);
	cvReleaseMat(&p3t);
	cvReleaseMat(&xp3t);
	cvReleaseMat(&yp3t);

	cvReleaseMat(&p1t2);
	cvReleaseMat(&p2t2);
	cvReleaseMat(&p3t2);
	cvReleaseMat(&xp3t2);
	cvReleaseMat(&yp3t2);

	cvReleaseMat(&a1);
	cvReleaseMat(&a2);
	cvReleaseMat(&a3);
	cvReleaseMat(&a4);

	return null(A);
}

CvPoint3D32f triangulateToPoint3D(const CvMat* P1, const CvMat* P2, const CvPoint2D32f& x1, const CvPoint2D32f& x2) {
	CvMat* hm = triangulate(P1, P2, x1, x2);
	CvPoint3D32f p = hmatToPoint3D(hm);
	cvReleaseMat(&hm);
	return p;
}

vector<CvPoint3D32f> triangulateToPoint3Ds(const CvMat* P1, const CvMat* P2, const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s) {

	vector<CvPoint3D32f> xs;
	for (int i = 0; i < x1s.size(); i++) {
		xs.push_back(triangulateToPoint3D(P1, P2, x1s[i], x2s[i]));
	}
	return xs;
}

//dstCol = s * srcCol
//s = (s0 * d0 + s1 * d1 + s2 * d2) / (s0 * s0 + s1 * s1 + s2 * s2)
double estimateScale(const CvMat* srcCol, const CvMat* dstCol) {
	double s0 = cvmGet(srcCol, 0, 0);
	double s1 = cvmGet(srcCol, 1, 0);
	double s2 = cvmGet(srcCol, 2, 0);

	double d0 = cvmGet(dstCol, 0, 0);
	double d1 = cvmGet(dstCol, 1, 0);
	double d2 = cvmGet(dstCol, 2, 0);

	return (s0 * d0 + s1 * d1 + s2 * d2) / (s0 * s0 + s1 * s1 + s2 * s2);
}

CvMat* estimateNtd(const CvMat* K1, const CvMat* K2, const CvMat* R, const CvMat* t, const CvMat* H) {

	CvMat* NH = matMul(inv(K2), H, K1);
	CvMat* RmNH = sub(R, NH);

	CvMat* NNH = scale(NH, 1/cvmGet(NH, 2, 2));
	CvMat* NR = scale(R, 1/cvmGet(R, 2, 2));
	CvMat* NRmNNH = sub(NR, NNH);

	cout<<"R: "<<R<<endl;
	cout<<"NH: "<<NH<<endl;
	cout<<"RmNH: "<<RmNH<<endl;

	cout<<"NR: "<<NR<<endl;
	cout<<"NNH: "<<NNH<<endl;
	cout<<"NRmNNH: "<<NRmNNH<<endl;

	CvMat* Ntd = cvCreateMat(1, 3, CV_64FC1);

	CvMat* c0 = getCol(NRmNNH, 0);
	CvMat* c1 = getCol(NRmNNH, 1);
	CvMat* c2 = getCol(NRmNNH, 2);

	cvmSet(Ntd, 0, 0, estimateScale(t, c0));
	cvmSet(Ntd, 0, 1, estimateScale(t, c1));
	cvmSet(Ntd, 0, 2, estimateScale(t, c2));

	cout<<"H: "<<H<<endl;
	cout<<"recovered H: "<<matMul(K2, sub(R, matMul(t, Ntd)), inv(K1))<<endl;

	cvReleaseMat(&NH);
	cvReleaseMat(&RmNH);

	cvReleaseMat(&c0);
	cvReleaseMat(&c1);
	cvReleaseMat(&c2);

	return Ntd;
}

	}
}