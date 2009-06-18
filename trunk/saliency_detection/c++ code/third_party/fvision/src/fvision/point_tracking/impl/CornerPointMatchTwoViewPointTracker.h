#ifndef FVISION_CORNER_POINT_MATCH_TWO_VIEW_POINT_TRACKER_H_
#define FVISION_CORNER_POINT_MATCH_TWO_VIEW_POINT_TRACKER_H_

#include "CornerDetector.h"
#include "PointMatcher.h"

#include "../TwoViewPointTracker.h"

namespace fvision {

class CornerPointMatchTwoViewPointTracker : public TwoViewPointTracker {
public:
	CornerPointMatchTwoViewPointTracker(CornerDetector* pcd, PointMatcher* ppm){
		this->pcd = pcd;
		this->ppm = ppm;
	}

	virtual ~CornerPointMatchTwoViewPointTracker() {
		delete pcd;
		delete ppm;
	}

public:
	void compute(const IplImage* im1, const IplImage* im2, std::vector<CvPoint2D32f>& x1s, std::vector<CvPoint2D32f>& x2s);

private:
	/**
	 * Input: two images
	 * pcorners1, feature points of first image, if it is NULL, the class compute it with pcd
	 * Output: point correspondences
	 */
	void compute(
		const IplImage* im1, 
		const IplImage* im2, 
		std::vector<CvPoint>& x1s, 
		std::vector<CvPoint>& x2s, 
		const std::vector<CvPoint>* pcorners1 = NULL);

public:
	const std::vector<CvPoint>& getCorners1() const {
		return corners1;
	}

	const std::vector<CvPoint>& getCorners2() const {
		return corners2;
	}

private:
	CornerDetector* pcd;
	PointMatcher* ppm;

private:
	std::vector<CvPoint> corners1;
	std::vector<CvPoint> corners2;
};

}
#endif // FVISION_CORNER_POINT_MATCH_TWO_VIEW_POINT_TRACKER_H_