
#include "GridCornerDetector.h"

namespace fvision {

void GridCornerDetector::compute(const IplImage* src, std::vector<CvPoint2D32f>& corners) {
	int halfInit = block_size / 2;
	for (int i = 0; i < src->height / block_size; i++) {
		for (int j = 0; j < src->width / block_size; j++) {
			corners.push_back(cvPoint2D32f(halfInit + j * block_size, halfInit + i * block_size));
		}
	}
}

}