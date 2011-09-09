#pragma once

#include <vector>

#include "highgui.h"
#include "cv.h"

#include "CvMatUtil.h"
#include "TwoViewGeometryUtil.h"

using namespace std;
using namespace ferry::cv_mat;

namespace ferry {
	namespace cv_geometry {
//
class Homography
{
public:
	Homography(CvMat* mat) {
		this->mat = mat;
	}

	~Homography() {
		cvReleaseMat(&mat);
	}

	string toString() {
		return CvMatUtil::matString(mat);
	}

	CvMat* getMat() {
		return mat;
	}

public:

	double transfromDistance(const CvPoint2D32f& p1, const CvPoint2D32f& p2) {
		return cvSqrt(transfromDistanceSquare(p1, p2));
	}

	double transfromDistanceSquare(const CvPoint2D32f& p1, const CvPoint2D32f& p2) {
		CvPoint2D32f tp1 = transform(p1);
		return (tp1.x - p2.x) * (tp1.x - p2.x) + (tp1.y - p2.y) * (tp1.y - p2.y);
	}

	CvPoint2D32f transform(const CvPoint2D32f& p) {
		CvMat* pm = CvMatUtil::hmatFromPoint2D(p);
		CvMat* tpm = cvCreateMat(3, 1, CV_64FC1);
		cvMatMulAdd(mat, pm, 0, tpm);
		//cout<<"mat: "<<CvMatUtil::matString(mat)<<endl;
		//cout<<"pm: "<<CvMatUtil::matString(pm)<<endl;
		//cout<<"tpm: "<<CvMatUtil::matString(tpm)<<endl;
		CvPoint2D32f tp = CvMatUtil::hmatToPoint2D(tpm);
		cvReleaseMat(&pm);
		cvReleaseMat(&tpm);
		return tp;
	}

	CvPoint2D32f transform(const CvPoint& p) {
		return transform(cvPointTo32f(p));
	}

private:
	CvMat* mat;
};

//model: homography
//sample: x1, x2
//
class HomographyInlierOutlierModel {

public:
	HomographyInlierOutlierModel(CvMat* H, double threshold) {
		this->H = H;
		this->threshold = threshold;
	}

public:
	void classify(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s, 
		          vector<CvPoint2D32f>& ix1s, vector<CvPoint2D32f>& ix2s, //inliers
				  vector<CvPoint2D32f>& ox1s, vector<CvPoint2D32f>& ox2s) { //outliers
					  for (int i = 0; i < (int)x1s.size(); i++) {
			cout<<computeHomographyDistance(H, x1s[i], x2s[i])<<endl;
						  if (computeHomographyDistance(H, x1s[i], x2s[i]) <= threshold) {
							  ix1s.push_back(x1s[i]);
							  ix2s.push_back(x2s[i]);
						  } else {
							  ox1s.push_back(x1s[i]);
							  ox2s.push_back(x2s[i]);
						  }
					  }
				  }

	void classifyToIndices(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s, 
		          vector<int>& is, vector<int>& os) {
					  for (int i = 0; i < (int)x1s.size(); i++) {
			cout<<computeHomographyDistance(H, x1s[i], x2s[i])<<endl;
						  if (computeHomographyDistance(H, x1s[i], x2s[i]) <= threshold) {
							  is.push_back(i);
						  } else {
							  os.push_back(i);
						  }
					  }
				  }

	int calcInliersNumber(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s) {
		int n = 0;
		for (int i = 0; i < (int)x1s.size(); i++) {
			cout<<computeHomographyDistance(H, x1s[i], x2s[i])<<endl;
			if (computeHomographyDistance(H, x1s[i], x2s[i]) <= threshold) {
				n++;
			}
		}
		return n;
	}
private:
	CvMat* H;
	double threshold;
};

	}
}