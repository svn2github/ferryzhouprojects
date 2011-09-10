#pragma once

#include "VideoFeatureTracker.h"
#include "FeatureTracking.h"

namespace ferry {
	namespace feature_tracking {

class SimpleTwoViewVFT : public VideoFeatureTracker
{
public:
	SimpleTwoViewVFT(CornerDetector* pcd, PointsOutFeatureMatcher* ppofm, int maxCornersNum, int minCornersNum)
		: VideoFeatureTracker(pcd, maxCornersNum, minCornersNum) {
		this->ppofm = ppofm;
	}

public:
	void doGetCorrespondences(vector<CvPoint>& x1s, vector<CvPoint>& x2s, int index1, int index2) {
		ppofm->compute(getImage(index1), getImage(index2), allCorners[index1], allCorners[index2], x1s, x2s);
	}

private:
	//FeatureMatcher* pfm;
	PointsOutFeatureMatcher* ppofm;
};

	}
}