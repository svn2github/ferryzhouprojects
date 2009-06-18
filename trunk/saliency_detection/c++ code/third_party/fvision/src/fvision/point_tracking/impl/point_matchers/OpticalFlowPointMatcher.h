#ifndef FVISION_OPTICAL_FLOW_POINT_MATCHER_H_
#define FVISION_OPTICAL_FLOW_POINT_MATCHER_H_

#include "../PointMatcher.h"

namespace fvision {

class OpticalFlowPointMatcher : public PointMatcher {

public:
	OpticalFlowPointMatcher(int halfSearchWindowSize, int halfPatchWindowSize, double threshold = 255) {
		this->halfSearchWindowSize = halfSearchWindowSize;
		this->halfPatchWindowSize = halfPatchWindowSize;
		this->threshold = threshold;
	}

public:
	/**
	for each corner in first image, search corresponding point from second image around the corner, ignore corners2
	*/
	void match(
		const IplImage* im1,              ///< first input image
		const IplImage* im2,                     ///< second input image
		const std::vector<CvPoint>& corners1,   ///< corners in first image, input
		const std::vector<CvPoint>& corners2,   ///< corners in second image, input
		std::vector<CvPoint>& x1s,              ///< output, point correspondences in first image
		std::vector<CvPoint>& x2s);             ///< output, point correspondences in second image

private:
	CvPoint search(
		const IplImage* im1, 
		const IplImage* im2,
		const CvPoint& p);

private:
	int halfSearchWindowSize;
	int halfPatchWindowSize;
	double threshold;
};

}
#endif // FVISION_OPTICAL_FLOW_POINT_MATCHER_H_