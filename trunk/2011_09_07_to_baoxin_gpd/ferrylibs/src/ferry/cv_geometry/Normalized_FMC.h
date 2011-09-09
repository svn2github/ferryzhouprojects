#pragma once

#include "FundamentalMatrixCalculator.h"
#include "PointsNormalizer.h"
#include "TwoViewGeometryUtil.h"
#include "CvMatUtil.h"

using namespace ferry::cv_mat;

/**
 * First normalized the points. i.e. make the center to the origin and
 * Then apply some algorithm to compute the fundamental matrix on the normalized point.
 * Finally, compute the fundamental matrix for the original points.
 */
namespace ferry {
	namespace cv_geometry {

class Normalized_FMC : public FundamentalMatrixCalculator
{
public:
	Normalized_FMC(FundamentalMatrixCalculator* pfmc) {
		this->pfmc = pfmc;
	}
	~Normalized_FMC(void) {}

public:
	FundamentalMatrix compute(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s) {
		vector<CvPoint2D32f> nx1s, nx2s;
		CvMat* T1 = PointsNormalizer::normalize(x1s, nx1s);
		CvMat* T2 = PointsNormalizer::normalize(x2s, nx2s);
		FundamentalMatrix nfm = pfmc->compute(nx1s, nx2s);
		CvMat* F = transF(nfm.getF(), T1, T2);
		return FundamentalMatrix(F);
	}

private:
	FundamentalMatrixCalculator* pfmc;
};

	}
}