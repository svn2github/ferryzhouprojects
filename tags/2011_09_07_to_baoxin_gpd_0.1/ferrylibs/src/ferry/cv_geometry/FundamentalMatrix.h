#pragma once

#include <vector>

#include "highgui.h"
#include "cv.h"

#include "CvMatUtil.h"
#include "BasicUtil.h"

using namespace std;
using namespace ferry::cv_mat;

namespace ferry {
	namespace cv_geometry {

class FundamentalMatrix
{
public:
	FundamentalMatrix(CvMat* mat) {
		this->mat = mat;
	}

	~FundamentalMatrix() {
		cvReleaseMat(&mat);
	}

	FundamentalMatrix clone() {
		return FundamentalMatrix(cloneMat(this->mat));
	}

	CvMat* getF() {
		return mat;
	}

	string toString() {
		return CvMatUtil::matString(mat);
	}

private:
	CvMat* mat;
};

//given F, x1, x2, compute error between F and (x1, x2)
class FundamentalMatrixErrorCalculator {
public:
	virtual double compute(CvMat* F, const CvPoint2D32f& x1, const CvPoint2D32f& x2) = 0;
};

class PointLine_FMEC : public FundamentalMatrixErrorCalculator {
public:
	//d(x2, Fx1) + d(x1, Ftx2)
	double compute(CvMat* F, const CvPoint2D32f& x1, const CvPoint2D32f& x2) {
		CvMat* mx1 = hmatFromPoint2D(x1);
		CvMat* mx2 = hmatFromPoint2D(x2);

		CvMat* Fx1 = matMul(F, mx1);

		CvMat* Ft = transpose(F);

		CvMat* Ftx2 = matMul(Ft, mx2);

		double d = point2LineDistance(mx2, Fx1) + point2LineDistance(mx1, Ftx2);

		//==DEBUG
		//cout<<"mx1: "<<mx1<<endl;
		//cout<<"Ftx2: "<<Ftx2<<endl;
		//cout<<"mx2: "<<mx2<<endl;
		//cout<<"Fx1: "<<Fx1<<endl;
		//cout<<"d: "<<d<<endl;
		//==

		cvReleaseMat(&mx1);
		cvReleaseMat(&mx2);
		cvReleaseMat(&Fx1);
		cvReleaseMat(&Ft);
		cvReleaseMat(&Ftx2);

		return d;
	}
};

class SimpleMultiply_FMEC : public FundamentalMatrixErrorCalculator {
public:
	//x2tFx1
	double compute(CvMat* F, const CvPoint2D32f& x1, const CvPoint2D32f& x2) {
		CvMat* mx1 = hmatFromPoint2D(x1);
		CvMat* mx2 = hmatFromPoint2D(x2);
		CvMat* mx2t = transpose(mx2);
		
		CvMat* D = matMul(mx2t, F, mx1);
		
		double d = abs(cvmGet(D, 0, 0));

		cvReleaseMat(&mx1);
		cvReleaseMat(&mx2);
		cvReleaseMat(&mx2t);
		cvReleaseMat(&D);

		return d;
	}
};

class FundamentalMatrixBatchErrorCalculator {
public:
	FundamentalMatrixBatchErrorCalculator(CvMat* F, FundamentalMatrixErrorCalculator* pfmec) {
		this->F = F;
		this->pfmec = pfmec;
	}

	double compute(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s) {
		int n = (int)x1s.size();
		double sum = 0;
		for (int i = 0; i < n; i++) {
			double d = pfmec->compute(F, x1s[i], x2s[i]);
			cout<<d<<endl;
			sum += d;
		}
		return sum / n;
	}

private:
	CvMat* F;
	FundamentalMatrixErrorCalculator* pfmec;
};

//model: fundamental matrix
//sample: x1, x2
//error measurement model: FundamentalMatrixErrorCalculator
class FundamentalMatrixInlierOutlierModel {

public:
	FundamentalMatrixInlierOutlierModel(CvMat* F, FundamentalMatrixErrorCalculator* pfmec, double threshold) {
		this->F = F;
		this->pfmec = pfmec;
		this->threshold = threshold;
	}

public:
	void classify(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s, 
		          vector<CvPoint2D32f>& ix1s, vector<CvPoint2D32f>& ix2s, //inliers
				  vector<CvPoint2D32f>& ox1s, vector<CvPoint2D32f>& ox2s) { //outliers
					  for (int i = 0; i < (int)x1s.size(); i++) {
						  if (pfmec->compute(F, x1s[i], x2s[i]) <= threshold) {
							  ix1s.push_back(x1s[i]);
							  ix2s.push_back(x2s[i]);
						  } else {
							  ox1s.push_back(x1s[i]);
							  ox2s.push_back(x2s[i]);
						  }
					  }
				  }

	int calcInliersNumber(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s) {
		int n = 0;
		for (int i = 0; i < (int)x1s.size(); i++) {
			//cout<<pfmec->compute(F, x1s[i], x2s[i])<<endl;
			if (pfmec->compute(F, x1s[i], x2s[i]) <= threshold) {
				n++;
			}
		}
		return n;
	}
private:
	CvMat* F;
	FundamentalMatrixErrorCalculator* pfmec;
	double threshold;
};

	}
}