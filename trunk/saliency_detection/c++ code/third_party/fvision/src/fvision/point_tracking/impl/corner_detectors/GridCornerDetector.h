#ifndef FVISION_GRID_CORNER_DETECTOR_H__
#define FVISION_GRID_CORNER_DETECTOR_H__

#include "../CornerDetector.h"

#include <cv.h>

namespace fvision {

class GridCornerDetector : public CornerDetector {
public:
	GridCornerDetector(int block_size) {
		this->block_size = block_size;
	}

public:
	void compute(const IplImage* src, std::vector<CvPoint2D32f>& corners);

private:
	int block_size;
};

}
#endif // FVISION_GRID_CORNER_DETECTOR_H__
