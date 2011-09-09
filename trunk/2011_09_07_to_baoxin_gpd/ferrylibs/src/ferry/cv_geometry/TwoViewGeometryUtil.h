#pragma once

#include <iostream>

#include <assert.h>

#include "cv.h"

#include "CvMatUtil.h"
#include <ferry/util/MathUtil.h>
#include "IOUtil.h"

using namespace std;
using namespace ferry::cv_mat;
using namespace ferry::util;
using namespace ferry::cv_geometry::io_util;

namespace ferry {
	namespace cv_geometry {

class TwoViewGeometryUtil
{
public:
	//F = e'xP'P+
	static CvMat* calcF(CvMat* P1, CvMat* P2) {
		
		CvMat* C = CvMatUtil::null(P1);
		cout<<"C "<<CvMatUtil::matString(C)<<endl;
		cout<<"P1 C: "<<CvMatUtil::matString(CvMatUtil::matMul(P1, C))<<endl;
		
		CvMat* e2 = CvMatUtil::matMul(P2, C);
		cout<<"e2: "<<CvMatUtil::matString(e2)<<endl;
		
		CvMat* e2x = CvMatUtil::getCrossMatrix(e2);
		CvMat* P1P = CvMatUtil::pseudoinverse(P1);

		CvMat* F = CvMatUtil::matMul(e2x, P2, P1P);

		cvReleaseMat(&C);
		cvReleaseMat(&e2);
		cvReleaseMat(&e2x);

		return F;
	}
};

//F = e'xP'P+
CvMat* calcF(CvMat* P1, CvMat* P2);

//suppose x1' = H1x1, x2' = H2x2
//and suppose F' is the fundamental matrix for {x1i', x2i'} such that x2i't F' x1i' = 0
//then F = H2t F'H1 is the fundamental matrix for {x1i, x2i} such that x2it F x1i = 0
CvMat* transF(CvMat* F, CvMat* H1, CvMat* H2);

//usually, the image coordinates is differnt from camera plane coordinates.
//the difference is that y' = h - 1 - y. h is the window height
//Therefore, there is a transformation: H = [1, 0, 0; 0, -1, h - 1; 0, 0, 1]
//such that F' = HtFH
CvMat* transFWithFlipY(CvMat* F, double h);

//x = Hx'
//H = [1/scale, 0, 0; 0, 1/scale, 0; 0, 0, 1]
CvMat* transFWithScale(CvMat* F, double scale);

//suppose x = MH x' which may mean transform an image to another kind of image such as scale or enlarge operation
//and suppose H is the homograhpy which make a transformation for an image such that xt = H x.
//then H' = MH-1 H MH is the homography for {x'} such that xt' = H' x'. 
CvMat* transH(CvMat* H, CvMat* MH);

//h is the window height
CvMat* transHWithFlipY(CvMat* H, double h);

//H = [1/scale, 0, 0; 0, 1/scale, 0; 0, 0, 1]
CvMat* transHWithScale(CvMat* H, double scale);

//H = [1, 0, 0; 0, -1, h - 1; 0, 0, 1]
CvMat* getHWithFlipY(double h);

//H = [1/scale, 0, 0; 0, 1/scale, 0; 0, 0, 1]
CvMat* getHWithScale(double scale);

CvMat* getHWithRotate(CvMat* K, double theta);

//x' = x + xv, y' = y + yv
//H = [1, 0, xv; 0, 1, yv; 0, 0, 1]
CvMat* getHWithTranslation(double xv, double yv);

//randomly sample N point pairs
void randomlySampleNPointPairs(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s, 
							   vector<CvPoint2D32f>& sx1s, vector<CvPoint2D32f>& sx2s, int n);

//F = [e2]xM
CvMat* getFM(CvMat* F, double a = 0.1);

//x' = H * x
CvPoint2D32f transformPoint(CvMat* H, const CvPoint2D32f& p);

vector<CvPoint2D32f> transformPoints(CvMat* H, const vector<CvPoint2D32f>& xs);

//x2 - Hx1
double computeHomographyDistance(CvMat* H, const CvPoint2D32f& x1, const CvPoint2D32f& x2);

CvMat* estimateEFromFK(const CvMat* F, const CvMat* K1, const CvMat* K2);

//[I|0]
CvMat* getStandardNP1();

CvMat* getP(const CvMat* K, const CvMat* R, const CvMat* t);

//[f, 0, width/2; 0, -f, height/2; 0, 0, 1] f = alpha * width
CvMat* getK(int width, int height, double alpha);

	}
}