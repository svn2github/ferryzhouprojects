#pragma once

#include <vector>

#include "highgui.h"
#include "cv.h"

#include "CvMatUtil.h"
#include "Camera.h"
#include <ferry/util/MathUtil.h>
#include "TwoViewGeometryUtil.h"

using namespace std;
using namespace ferry::cv_mat;
using namespace ferry::util;

namespace ferry {
	namespace cv_geometry {

class TwoViewSimulation
{
public:
	TwoViewSimulation(vector<CvPoint3D32f>& Xs, Camera* cam1, Camera* cam2, bool addNoise = false, double sigma = 0.0) {
		setModel(Xs);
		setCameras(cam1, cam2);
		calcImagePoints();
		if (addNoise) addSomeNoise(sigma);
	}

	~TwoViewSimulation(void) {
	}

private:
	void setModel(vector<CvPoint3D32f>& Xs) {
		p3ds = Xs;
		mp3ds = CvMatUtil::vectorHmatFromPoint3D(p3ds);
	}

	void setCameras(Camera* cam1, Camera* cam2) {
		this->pCamera1 = cam1;
		this->pCamera2 = cam2;
	}

	void calcImagePoints() {
		x1s = pCamera1->calcImagePoints(p3ds);
		x2s = pCamera2->calcImagePoints(p3ds);

		mx1s = CvMatUtil::vectorHmatFromPoint2D(x1s);
		mx2s = CvMatUtil::vectorHmatFromPoint2D(x2s);
	}

	//assume mean is 0;
	//add some noise to image points
	void addSomeNoise(double sigma) {
		for (int i = 0; i < (int)x1s.size(); i++) {
			(x1s[i]).x += gaussianRand(0.0, sigma);
			(x1s[i]).y += gaussianRand(0.0, sigma);
			(x2s[i]).x += gaussianRand(0.0, sigma);
			(x2s[i]).y += gaussianRand(0.0, sigma);
		}
		releaseVectorMats(&mx1s);
		releaseVectorMats(&mx2s);
		mx1s = CvMatUtil::vectorHmatFromPoint2D(x1s);
		mx2s = CvMatUtil::vectorHmatFromPoint2D(x2s);
	}

public:
	void generateGroundTruth() {
		genEpipoles();
		genF();
	}


public:
	//calc 3d points in cam1 coordinate system
	void calcCam1Points() {
		pcam1p3ds = pCamera1->calcCoordPoints(p3ds);
		mpcam1p3ds = CvMatUtil::vectorHmatFromPoint3D(pcam1p3ds);
	}

	vector<CvPoint3D32f> getCam1Points() {
		return pcam1p3ds;
	}

public:
	CvMat* getP1() {
		return pCamera1->getP();
	}

	CvMat* getP2() {
		return pCamera2->getP();
	}

	CvMat* getF() {
		return F;
	}

	vector<CvPoint3D32f> getXs() {
		return p3ds;
	}

	vector<CvPoint2D32f> getx1s() {
		return x1s;
	}

	vector<CvPoint2D32f> getx2s() {
		return x2s;
	}

	CvPoint2D32f getEpipole(int index) {
		if (index == 1) return e1;
		else return e2;
	}

public:
	void validateF(CvMat* F) {
		for (int i = 0; i < (int)x1s.size(); i++) {
			//cout<<"x2 "<<Matrix2DUtil::transpose(x2);
			//cout<<"x1 "<<Matrix2DUtil::transpose(x1);
			cout<<"x2tFx1 "<<CvMatUtil::matString(CvMatUtil::matMul(CvMatUtil::transpose(mx2s[i]), F, mx1s[i]))<<endl;
		}
	}

private:
	void genEpipoles() {
		e1 = pCamera1->calcImagePoint(pCamera2->getHC());
		e2 = pCamera2->calcImagePoint(pCamera1->getHC());

		me1 = CvMatUtil::hmatFromPoint2D(e1);
		me2 = CvMatUtil::hmatFromPoint2D(e2);
		//==DEBUG
		cout<<"e1: "<<e1.x<<", "<<e1.y<<endl;
		cout<<"e2: "<<e2.x<<", "<<e2.y<<endl;
		//==
	}

	void genF() {
		F = TwoViewGeometryUtil::calcF(pCamera1->getP(), pCamera2->getP());
		
		//==DEBUG
		CvMat* Fe1 = CvMatUtil::matMul(F, me1);
		CvMat* Ft = cvCreateMat(F->cols, F->rows, F->type);
		cvT(F, Ft);
		CvMat* Fte2 = CvMatUtil::matMul(Ft, me2);
		cout<<"Fe1 "<<CvMatUtil::matString(Fe1)<<endl;
		cout<<"Fte2 "<<CvMatUtil::matString(Fte2)<<endl;
		//==
	}

private:
	void releaseVectorMats(vector<CvMat*>* pms) {
		for (int i = 0; i < (int)pms->size(); i++) {
			cvReleaseMat(&((*pms)[i]));
		}
		pms->clear();
	}

private:
	Camera* pCamera1;
	Camera* pCamera2;

	//epipole
	CvPoint2D32f e1;
	CvPoint2D32f e2;

	vector<CvPoint3D32f> p3ds;

	vector<CvPoint2D32f> x1s;
	vector<CvPoint2D32f> x2s;

	vector<CvPoint3D32f> pcam1p3ds;

	CvMat* F;

	//matrix form
	CvMat* me1;
	CvMat* me2;

	vector<CvMat*> mp3ds;

	vector<CvMat*> mx1s;
	vector<CvMat*> mx2s;

	vector<CvMat*> mpcam1p3ds;

};

	}
}