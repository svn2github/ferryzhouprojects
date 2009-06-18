#pragma once

#include "PointTransformer.h"

namespace fvision {

//x' = K * x
//x' = fx * x + px, y' = fy * y + py
class KDeNormalizer : public PointTransformer {
public:
	KDeNormalizer(const CvMat* K) {
		px = cvmGet(K, 0, 2);
		py = cvmGet(K, 1, 2);
		fx = cvmGet(K, 0, 0);
		fy = cvmGet(K, 1, 1);
	}

	CvPoint2D32f transform(const CvPoint2D32f& p) const {
		return cvPoint2D32f(fx * p.x + px, fy * p.y + py);
	}

private:
	double px;
	double py;
	double fx;
	double fy;
};

}