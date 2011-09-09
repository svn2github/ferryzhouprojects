#pragma once

#include <cassert>

#include "HomographyCalculator.h"
#include "CvMatUtil.h"
#include "DLT_HC.h"
#include "RANSAC.h"
#include "TwoViewGeometryUtil.h"

using namespace ferry::cv_mat;
using namespace ferry::math;

namespace ferry {
	namespace cv_geometry {

class RANSAC_HC
{
public:
	RANSAC_HC(double threshold, int modelSampleSize = 5) {
		this->threshold = threshold;
		this->modelSampleSize = modelSampleSize;
	}

	~RANSAC_HC(void) {}

public:
	Homography compute(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s) {
		assert(x1s.size() >= 4 && x1s.size() == x2s.size()); 

		int N = 1000;
		int sampleCount = 0;
		int pointsNumber = (int)x1s.size();
		RANSAC ransac(modelSampleSize);
		
		CvMat* bestH = NULL;
		int maxInliersNumber = 0;
		
		while ((sampleCount++) < N) {
			cout<<"iteration: "<<sampleCount<<endl;
			//select a random sample -- 5 point correspondences
			vector<CvPoint2D32f> sx1s, sx2s;
			randomlySampleNPointPairs(x1s, x2s, sx1s, sx2s, modelSampleSize);
			cout<<"sx1s: "<<sx1s<<endl;
			cout<<"sx2s: "<<sx2s<<endl;

			Homography TH = hc.compute(sx1s, sx2s);
			CvMat* sH = TH.getMat();
			cout<<"sH: "<<sH<<endl;

			HomographyInlierOutlierModel iom(sH, threshold);
			int inliersNumber = iom.calcInliersNumber(x1s, x2s);
			cout<<"inliers number: "<<inliersNumber<<endl;

			if (inliersNumber > maxInliersNumber) {
				bestH = cloneMat(sH);
				maxInliersNumber = inliersNumber;
				N = ransac.calcN(inliersNumber, pointsNumber);
			}

			cout<<"N: "<<N<<endl;
		}

		cout<<"sampleCount: "<<sampleCount<<endl;

		HomographyInlierOutlierModel iom(bestH, threshold);
		iom.classify(x1s, x2s, ix1s, ix2s, ox1s, ox2s);
		cout<<"inliers num: "<<(int)ix1s.size()<<endl;
		return hc.compute(ix1s, ix2s);
	}

	vector<CvPoint2D32f> getix1s() {
		return ix1s;
	}

	vector<CvPoint2D32f> getix2s() {
		return ix2s;
	}

	vector<CvPoint2D32f> getox1s() {
		return ox1s;
	}

	vector<CvPoint2D32f> getox2s() {
		return ox2s;
	}

private:
	double threshold;
	int modelSampleSize;
	DLT_HC hc;

private:
	vector<CvPoint2D32f> ix1s;
	vector<CvPoint2D32f> ix2s;
	vector<CvPoint2D32f> ox1s;
	vector<CvPoint2D32f> ox2s;
};

	}
}