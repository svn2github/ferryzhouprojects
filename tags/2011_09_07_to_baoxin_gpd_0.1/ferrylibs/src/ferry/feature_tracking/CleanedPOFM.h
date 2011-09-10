#pragma once

#include "PointsOutFeatureMatcher.h"
#include "MatchCleaner.h"

namespace ferry {
	namespace feature_tracking {

class CleanedPOFM : public PointsOutFeatureMatcher
{
public:
	CleanedPOFM(PointsOutFeatureMatcher* ppofm, MatchCleaner* pmc) {
		this->ppofm = ppofm;
		this->pmc = pmc;
	}

	~CleanedPOFM(void);

public:
	void compute(IplImage* im1, IplImage* im2, const vector<CvPoint>& corners1, const vector<CvPoint>& corners2, vector<CvPoint>& x1s, vector<CvPoint>& x2s) {
		vector<CvPoint> init_x1s, init_x2s;
		ppofm->compute(im1, im2, corners1, corners2, init_x1s, init_x2s);
		pmc->clean(init_x1s, init_x2s, x1s, x2s);
	}
	
private:
	PointsOutFeatureMatcher* ppofm;
	MatchCleaner* pmc;
};

	}
}