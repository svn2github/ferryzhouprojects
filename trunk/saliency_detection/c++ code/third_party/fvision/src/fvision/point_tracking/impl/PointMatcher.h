#ifndef FVISION_POINT_MATCHER_H_
#define FVISION_POINT_MATCHER_H_

#include <cv.h>

#include <vector>

namespace fvision {

class PointMatcher {
public:
	virtual ~PointMatcher() {}

public:
	//compute point correspondences based on input points
	virtual void match(
		const IplImage* im1,      ///< first input image
		const IplImage* im2,                     ///< second input image
		const std::vector<CvPoint>& corners1,   ///< corners in first image, input
		const std::vector<CvPoint>& corners2,   ///< corners in second image, input
		std::vector<CvPoint>& x1s,              ///< output, point correspondences in first image
		std::vector<CvPoint>& x2s) = 0;         ///< output, point correspondences in second image
};

}
#endif // FVISION_POINT_MATCHER_H_