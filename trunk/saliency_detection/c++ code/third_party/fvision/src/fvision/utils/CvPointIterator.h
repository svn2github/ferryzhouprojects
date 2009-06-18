#pragma once

#include <cv.h>

namespace fvision {
class CvPointIterator {
public:
	virtual bool hasNext() const = 0;
	virtual const CvPoint& next() = 0;
};

template <typename UnaryOperator>
void doForEachPoint(CvPointIterator* iter, UnaryOperator& op) {
	while (iter->hasNext()) {
		op(iter->next());
	}
}

}