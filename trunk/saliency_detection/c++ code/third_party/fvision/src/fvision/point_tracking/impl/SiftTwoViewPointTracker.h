#ifndef FVISION_SIFT_TWO_VIEW_POINT_TRACKER_H_
#define FVISION_SIFT_TWO_VIEW_POINT_TRACKER_H_

#include "../TwoViewPointTracker.h"

namespace fvision {

/** SIFT method
 *
 */
class SiftTwoViewPointTracker : public TwoViewPointTracker {
public:
	SiftTwoViewPointTracker(void);
public:
	~SiftTwoViewPointTracker(void);

public:
	void compute(const IplImage* im1, const IplImage* im2, std::vector<CvPoint2D32f>& x1s, std::vector<CvPoint2D32f>& x2s);

};

}
#endif // FVISION_SIFT_TWO_VIEW_POINT_TRACKER_H_