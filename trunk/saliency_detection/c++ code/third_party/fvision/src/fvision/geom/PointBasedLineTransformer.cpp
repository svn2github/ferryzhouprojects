#include "PointBasedLineTransformer.h"

#include <cassert>

namespace fvision {

PointBasedLineTransformer::PointBasedLineTransformer(const PointTransformer* pointTransformer) {
	assert(pointTransformer != NULL);
	this->pointTransformer = pointTransformer;
}

PointBasedLineTransformer::~PointBasedLineTransformer(void) {
	//delete pointTransformer;
}

Line PointBasedLineTransformer::transform(const Line& line) const {
	return Line(pointTransformer->transform(line.getPoint1()),
		pointTransformer->transform(line.getPoint2()));
}

}
