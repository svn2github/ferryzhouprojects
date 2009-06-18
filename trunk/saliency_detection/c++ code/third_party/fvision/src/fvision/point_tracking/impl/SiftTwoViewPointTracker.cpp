#include "SiftTwoViewPointTracker.h"

#include "../sift/sift_wrapper.h"

namespace fvision {

SiftTwoViewPointTracker::SiftTwoViewPointTracker(void)
{
}

SiftTwoViewPointTracker::~SiftTwoViewPointTracker(void)
{
}

void SiftTwoViewPointTracker::compute(const IplImage* im1, const IplImage* im2, std::vector<CvPoint2D32f>& x1s, std::vector<CvPoint2D32f>& x2s) {
	sift_compute(im1, im2, x1s, x2s);
}

}