#ifndef FVISION_CORNER_DETECTOR_H_
#define FVISION_CORNER_DETECTOR_H_

#include <cv.h>

#include <vector>

namespace fvision {

class CornerDetector {
public:
	virtual ~CornerDetector() {}

	virtual void compute(const IplImage* src, std::vector<CvPoint2D32f>& corners) = 0;

	void compute(const IplImage* src, std::vector<CvPoint>& corners);
};

}
#endif // FVISION_CORNER_DETECTOR_H_
