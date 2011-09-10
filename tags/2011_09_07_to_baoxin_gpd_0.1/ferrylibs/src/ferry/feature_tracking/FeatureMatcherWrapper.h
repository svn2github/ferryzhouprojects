#pragma once

#include "FeatureMatcher.h"
#include "Index2PointConverter.h"
#include "PointsOutFeatureMatcher.h"

namespace ferry {
	namespace feature_tracking {

class FeatureMatcherWrapper : public PointsOutFeatureMatcher
{
public:
	FeatureMatcherWrapper(FeatureMatcher* pfm) {
		this->pfm = pfm;
	}

public:
	void compute(IplImage* im1, IplImage* im2, const vector<CvPoint>& corners1, const vector<CvPoint>& corners2, vector<CvPoint>& x1s, vector<CvPoint>& x2s) {
		int co1[MAX_FEATURE_NUM];
		int co2[MAX_FEATURE_NUM];
		pfm->compute(im1, im2, corners1, corners2, co1, co2);
		Index2PointConverter::convert(corners1, corners2, co1, co2, x1s, x2s);
	}

private:
	FeatureMatcher* pfm;
};

	}
}