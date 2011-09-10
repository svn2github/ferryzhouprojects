#pragma once

#include <assert.h>

#include "FundamentalMatrixCalculator.h"
#include "DLT_FMC.h"
#include "RANSAC.h"
#include "CvMatUtil.h"
#include "TwoViewGeometryUtil.h"

using namespace ferry::cv_mat;
using namespace ferry::math;

namespace ferry {
	namespace cv_geometry {

class RANSAC_FMC : public FundamentalMatrixCalculator
{
public:
	RANSAC_FMC(double threshold, int modelSampleSize = 8) {
		this->threshold = threshold;
		this->modelSampleSize = modelSampleSize;
		this->pfmec = new PointLine_FMEC();
	}

	~RANSAC_FMC(void) {}

public:
	FundamentalMatrix compute(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s) {
		int N = 1.0e+10;
		int sampleCount = 0;
		int pointsNumber = (int)x1s.size();
		RANSAC ransac(modelSampleSize);
		
		CvMat* bestF;
		int maxInliersNumber = 0;
		
		while (sampleCount < N) {
			//select a random sample -- 8 point correspondences
			vector<CvPoint2D32f> sx1s, sx2s;
			randomlySampleNPointPairs(x1s, x2s, sx1s, sx2s, modelSampleSize);

			FundamentalMatrix sF = fmc.compute(sx1s, sx2s);
			//cout<<"sF: "<<sF<<endl;

			FundamentalMatrixInlierOutlierModel fmiom(sF.getF(), pfmec, threshold);
			int inliersNumber = fmiom.calcInliersNumber(x1s, x2s);
			cout<<"inliers number: "<<inliersNumber<<endl;

			if (inliersNumber > maxInliersNumber) {
				bestF = cloneMat(sF.getF());
				maxInliersNumber = inliersNumber;
				N = ransac.calcN(inliersNumber, pointsNumber);
			}

			cout<<"N: "<<N<<endl;

			sampleCount ++;
		}

		cout<<"sampleCount: "<<sampleCount<<endl;

		FundamentalMatrixInlierOutlierModel fmiom(bestF, pfmec, threshold);
		vector<CvPoint2D32f> ix1s, ix2s, ox1s, ox2s;
		fmiom.classify(x1s, x2s, ix1s, ix2s, ox1s, ox2s);
		cout<<"inliers num: "<<(int)ix1s.size()<<endl;
		return fmc.compute(ix1s, ix2s);
	}

public:
	double threshold;
	int modelSampleSize;
	DLT_FMC fmc;
	FundamentalMatrixErrorCalculator* pfmec;
};

	}
}