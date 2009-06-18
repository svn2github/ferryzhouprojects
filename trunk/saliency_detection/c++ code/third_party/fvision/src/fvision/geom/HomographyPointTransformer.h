#pragma once

#include "PointTransformer.h"

namespace fvision {

class HomographyPointTransformer : public PointTransformer {
public:
	HomographyPointTransformer(const CvMat* H);
	~HomographyPointTransformer(void);

public:
	CvPoint2D32f transform(const CvPoint2D32f& p) const;

private:
	CvMat* H;

	CvMat* inputPointBuffer;
	CvMat* outputPointBuffer;
};

}