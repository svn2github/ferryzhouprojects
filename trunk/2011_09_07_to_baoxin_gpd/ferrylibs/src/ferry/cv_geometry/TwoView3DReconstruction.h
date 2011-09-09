#pragma once

#include <fstream>

#include "TwoViewGeometryUtil.h"
#include "Camera.h"

using namespace std;

namespace ferry {
	namespace cv_geometry {

CvMat* triangulate(const CvMat* P1, const CvMat* P2, const CvPoint2D32f& x1, const CvPoint2D32f& x2);
CvPoint3D32f triangulateToPoint3D(const CvMat* P1, const CvMat* P2, const CvPoint2D32f& x1, const CvPoint2D32f& x2);
vector<CvPoint3D32f> triangulateToPoint3Ds(const CvMat* P1, const CvMat* P2, const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s);

//H = K2*(R - t*Nt/d)*invK1
CvMat* estimateNtd(const CvMat* K1, const CvMat* K2, const CvMat* R, const CvMat* t, const CvMat* H);

class ECalibrator {
public:
	ECalibrator(CvMat* F, CvMat* K1, CvMat* K2, CvPoint2D32f x1, CvPoint2D32f x2) {
		this->F = F;
		this->K1 = K1;
		this->K2 = K2;
		this->x1 = x1;
		this->x2 = x2;

		this->pcam1 = new Camera(K1, getStandardNP1());
		this->pcam2 = NULL;

		this->W = cvCreateMat(3, 3, CV_64FC1);
		cvZero(W);
		cvmSet(W, 0, 1, -1);
		cvmSet(W, 1, 0, 1);
		cvmSet(W, 2, 2, 1);
	}

public:
	void compute() {
		cout<<"K1: "<<K1<<endl;
		cout<<"F: "<<F<<endl;
		cvScale(F, F, 1000);
		E = estimateEFromFK(F, K1, K2);
		CvMat* U;
		CvMat* V;
		CvMat* W2;
		svd(E, &W2, &U, &V);

		CvMat* u3 = getCol(V, 2);
		CvMat* mu3 = scale(u3, -1);

		CvMat* Vt = transpose(V);
		CvMat* Wt = transpose(W);
		CvMat* UWVt = matMul(U, W, Vt);
		CvMat* UWtVt = matMul(U, Wt, Vt);
		
		if (cvDet(UWVt) < 0) cvScale(UWVt, UWVt, -1);
		if (cvDet(UWtVt) < 0) cvScale(UWtVt, UWtVt, -1);

		cout<<"E: "<<E<<endl;
		cout<<"W2: "<<W2<<endl;
		cout<<"U: "<<U<<endl;
		cout<<"V: "<<V<<endl;
		cout<<"UWVt: "<<UWVt<<endl;
		cout<<"UWtVt: "<<UWtVt<<endl;
		cout<<"u3: "<<u3<<endl;
		//==1
		if (isValid(getP(K2, UWVt, u3))) {
			pcam2 = new Camera(K2, cloneMat(UWVt), cloneMat(u3));
		} else if (isValid(getP(K2, UWVt, mu3))) {
			pcam2 = new Camera(K2, cloneMat(UWVt), cloneMat(mu3));
		} else if (isValid(getP(K2, UWtVt, u3))) {
			pcam2 = new Camera(K2, cloneMat(UWtVt), cloneMat(u3));
		} else if (isValid(getP(K2, UWtVt, mu3))) {
			pcam2 = new Camera(K2, cloneMat(UWtVt), cloneMat(mu3));
		}

		cvReleaseMat(&U);
		cvReleaseMat(&W2);
		cvReleaseMat(&V);

		cvReleaseMat(&u3);
		cvReleaseMat(&mu3);

		cvReleaseMat(&Vt);
		cvReleaseMat(&Wt);
		cvReleaseMat(&UWVt);
		cvReleaseMat(&UWtVt);
	}

	CvMat* getR() {
		return pcam2->getR();
	}

	CvMat* gett() {
		return pcam2->gett();
	}

private:
	bool isValid(const CvMat* P2) {
		CvMat* P1 = pcam1->getP();
		CvMat* mX = triangulate(P1, P2, x1, x2);
		if (cvmGet(mX, 3, 0) < 0) cvScale(mX, mX, -1);
		CvMat* mx1 = matMul(P1, mX);
		CvMat* mx2 = matMul(P2, mX);
		cout<<"P2: "<<P2<<endl;
		cout<<"mX: "<<mX<<endl;
		cout<<"mx1: "<<mx1<<", "<<::hmatToPoint2D(mx1)<<endl;
		cout<<"mx2: "<<mx2<<", "<<::hmatToPoint2D(mx2)<<endl;
		bool valid = (cvmGet(mx1, 2, 0) > 0) && (cvmGet(mx2, 2, 0) > 0);
		cvReleaseMat(&mX);
		cvReleaseMat(&mx1);
		cvReleaseMat(&mx2);
		return valid;
	}

private:
	CvMat* F;
	CvMat* K1;
	CvMat* K2;
	CvPoint2D32f x1;
	CvPoint2D32f x2;

	CvMat* E;
	Camera* pcam1;
	Camera* pcam2;

	CvMat* W;
};

	}
}