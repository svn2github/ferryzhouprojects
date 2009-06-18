#pragma once

#include "PointTransformer.h"

namespace fvision {

class PointTranslator : public PointTransformer {
public:
	PointTranslator(const CvPoint2D32f& t) {
		this->t = t;
	}

public:
	CvPoint2D32f transform(const CvPoint2D32f& p) const {
		return cvPoint2D32f(p.x + t.x, p.y + t.y);
	}

private:
	CvPoint2D32f t;
};

}