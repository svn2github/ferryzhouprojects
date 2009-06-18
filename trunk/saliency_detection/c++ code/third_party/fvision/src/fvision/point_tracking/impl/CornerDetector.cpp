#include "CornerDetector.h"

namespace fvision {

void CornerDetector::compute(const IplImage* src, std::vector<CvPoint>& corners) {
	std::vector<CvPoint2D32f> xs;
	compute(src, xs);
	for (size_t i = 0; i < xs.size(); i++) {
		corners.push_back(cvPointFrom32f(xs[i]));
	}
}

}
