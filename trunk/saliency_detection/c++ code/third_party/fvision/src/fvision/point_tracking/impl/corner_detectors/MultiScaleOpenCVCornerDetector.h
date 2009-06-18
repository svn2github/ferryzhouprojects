#ifndef FVISION_MULTI_SCALE_OPENCV_CORNER_DETECTOR_H_
#define FVISION_MULTI_SCALE_OPENCV_CORNER_DETECTOR_H_

#include "../CornerDetector.h"

namespace fvision {

class MultiScaleOpenCVCornerDetector : public CornerDetector {
public:
	MultiScaleOpenCVCornerDetector(CornerDetector* pcd) {
		this->pcd = pcd;
	}

public:
	~MultiScaleOpenCVCornerDetector(void) {}

public:
	void compute(const IplImage* src, std::vector<CvPoint2D32f>& corners);

private:
	CornerDetector* pcd;
};

}
#endif // FVISION_MULTI_SCALE_OPENCV_CORNER_DETECTOR_H_