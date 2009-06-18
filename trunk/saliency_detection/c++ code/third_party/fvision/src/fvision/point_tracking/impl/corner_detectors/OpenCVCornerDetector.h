#ifndef FVISION_OPENCV_CORNER_DETECTOR_H_
#define FVISION_OPENCV_CORNER_DETECTOR_H_

#include "../CornerDetector.h"

namespace fvision {

/**
 * see OpenCV cvGoodFeaturesToTrack
 */
class OpenCVCornerDetector : public CornerDetector
{
public:
	OpenCVCornerDetector(int corners_count, double quality_level, double min_distance) {
		this->corners_count = corners_count;
		this->quality_level = quality_level;
		this->min_distance = min_distance;
	}

public:
	void compute(const IplImage* src, std::vector<CvPoint2D32f>& corners);

private:
	int corners_count;
	double quality_level;
	double min_distance;

};

}
#endif // FVISION_OPENCV_CORNER_DETECTOR_H_