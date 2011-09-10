#pragma once

#include "cv.h"

#include "TwoViewSimulation.h"

using namespace ferry::geometry;

namespace ferry {
	namespace feature_tracking {

class CvPointMatrix2DConverter
{
public:
	static void cvPoints2Matrix2Ds(const vector<CvPoint>& cpxs, vector<Matrix2D>& mxs, double dividenum = 100) {
		for (int i = 0; i < (int)cpxs.size(); i++) {
			mxs.push_back(cvPoint2Matrix2D(cpxs[i], dividenum));
		}
	}

	static Matrix2D cvPoint2Matrix2D(const CvPoint& cp, double dividenum = 100) {
		return Matrix2DUtil::initPoint2D((double)(cp.x) / dividenum, (double)cp.y / dividenum);
	}

	static void matrix2Ds2CvPoints(vector<Matrix2D>& mxs, vector<CvPoint>& cpxs, double multiplynum = 100) {
		int count = (int)mxs.size();
		for (int i = 0; i < count; i++) {
			cpxs.push_back(matrix2D2CvPoint(mxs[i], multiplynum));
		}
	}

	static CvPoint matrix2D2CvPoint(Matrix2D& mx, double multiplynum = 100) {
		return cvPoint(mx[0][0] * multiplynum, mx[1][0] * multiplynum);
	}
};

class CvPointMatrix2DConvertInterface {
public:
	void cvPoints2Matrix2Ds(const vector<CvPoint>& cpxs, vector<Matrix2D>& mxs) {
		for (int i = 0; i < (int)cpxs.size(); i++) {
			mxs.push_back(cvPoint2Matrix2D(cpxs[i]));
		}
	}

	void matrix2Ds2CvPoints(vector<Matrix2D>& mxs, vector<CvPoint>& cpxs) {
		int count = (int)mxs.size();
		for (int i = 0; i < count; i++) {
			cpxs.push_back(matrix2D2CvPoint(mxs[i]));
		}
	}

	virtual CvPoint matrix2D2CvPoint(Matrix2D& mx) = 0;

	virtual Matrix2D cvPoint2Matrix2D(const CvPoint& cp) = 0;

};

class WindowPMC : public CvPointMatrix2DConvertInterface {
public:
	WindowPMC(int windowWidth, int windowHeight) {
		this->halfWindowHeight = windowHeight / 2;
		this->halfWindowWidth = windowWidth / 2;
		this->maxLength = (halfWindowHeight > halfWindowWidth) ? halfWindowHeight : halfWindowWidth;
	}

public:
	CvPoint matrix2D2CvPoint(Matrix2D& mx) {
		return cvPoint(mx[0][0] * maxLength + halfWindowWidth, halfWindowHeight - mx[1][0] *maxLength);		
	}

	Matrix2D cvPoint2Matrix2D(const CvPoint& cp) {
		return Matrix2DUtil::initPoint2D((double)(cp.x - halfWindowWidth) / maxLength, (double)(halfWindowHeight - cp.y) / maxLength);
	}

	//mp = M * P
	Matrix2D getM() {
		Matrix2D M = Matrix2D(3, 3, 0.0f);
		M[0][0] = (double)1 / maxLength;
		M[0][2] = - (double)halfWindowWidth / maxLength;
		M[1][1] = -(double)1/maxLength;
		M[1][2] = (double)halfWindowHeight/maxLength;
		M[2][2] = 1;
		return M;
	}

	//P = M-1 * mp
	Matrix2D getMT() {
		Matrix2D M = Matrix2D(3, 3, 0.0f);
		M[0][0] = maxLength;
		M[0][2] = halfWindowWidth;
		M[1][1] = -maxLength;
		M[1][2] = halfWindowHeight;
		M[2][2] = 1;
		return M;
	}

private:
	int halfWindowHeight;
	int halfWindowWidth;

	int maxLength;
};

class ScaleFMC : public CvPointMatrix2DConvertInterface {
public:
	ScaleFMC(double scale) {
		this->scale = scale;
	}

public:
	CvPoint matrix2D2CvPoint(Matrix2D& mx) {
		return cvPoint(mx[0][0] * scale, mx[1][0] * scale);
	}

	Matrix2D cvPoint2Matrix2D(const CvPoint& cp) {
		return Matrix2DUtil::initPoint2D((double)(cp.x) / scale, (double)cp.y / scale);
	}

private:
	double scale;
};

	}
}