#ifndef FVISION_SIMPLE_WINDOW_POINT_MATCHER_H_
#define FVISION_SIMPLE_WINDOW_POINT_MATCHER_H_

#include "../PointMatcher.h"

namespace fvision {

/**
 * block match
 */
class SimpleWindowPointMatcher : public PointMatcher {

public:
	SimpleWindowPointMatcher(double maxDistance, int halfWindowSize, double threshold, int halfNeighborSize = 0, int type = 0) {
		this->maxDistance = maxDistance;
		this->halfWindowSize = halfWindowSize;
		this->threshold = threshold;
		this->halfNeighborSize = halfNeighborSize;
		this->type = type;
	}

public:
	/**
	for each corner in first image, search corresponding corner from second image within a range
	first it store a set of possible matches, i.e. matching score is less than the threshold
	second whether the minimum matching score is significant

	for each feature, multi-scale, 
	*/
	void match(
		const IplImage* im1,              ///< first input image
		const IplImage* im2,                     ///< second input image
		const std::vector<CvPoint>& corners1,   ///< corners in first image, input
		const std::vector<CvPoint>& corners2,   ///< corners in second image, input
		std::vector<CvPoint>& x1s,              ///< output, point correspondences in first image
		std::vector<CvPoint>& x2s);             ///< output, point correspondences in second image

private:
	/**
	find a match for p1 from corners2
	may fail and return CvPoint(-1, -1)
	*/
	CvPoint search(
		const IplImage* im1, 
		const IplImage* im2,
		const CvPoint& p1,
		const std::vector<CvPoint>& corners2);

private:
	double maxDistance;
	int halfWindowSize;
	int halfNeighborSize;
	double threshold;

	int type; //0 - gray; 1 - color (BGR); 2 - hs(v) ; 3 - rg(b);


};

}
#endif // FVISION_SIMPLE_WINDOW_POINT_MATCHER_H_