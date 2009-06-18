#pragma once

#include "PointTransformer.h"

namespace fvision {

//x' = invK * x
//x = ifx * (x - px), y = ify * (y - py)
class KNormalizer : public PointTransformer {
public:
	KNormalizer(const CvMat* K) {
		px = cvmGet(K, 0, 2);
		py = cvmGet(K, 1, 2);
		ifx = 1 / cvmGet(K, 0, 0);
		ify = 1 / cvmGet(K, 1, 1);
	}

	CvPoint2D32f transform(const CvPoint2D32f& p) const {
		return cvPoint2D32f(ifx * (p.x - px), ify * (p.y - py));
	}

private:
	double px;
	double py;
	double ifx;
	double ify;
};

}