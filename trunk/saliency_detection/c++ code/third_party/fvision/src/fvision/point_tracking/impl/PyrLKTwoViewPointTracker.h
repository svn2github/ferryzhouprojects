#ifndef FVISION_PYR_LK_TWO_VIEW_POINT_TRACKER_H_
#define FVISION_PYR_LK_TWO_VIEW_POINT_TRACKER_H_

#include "CornerDetector.h"
#include "../TwoViewPointTracker.h"

namespace fvision {

/** PyrLK method
 * 
 * Parameters:
 *    CornerDetector
 * 
 */
class PyrLKTwoViewPointTracker : public TwoViewPointTracker{
public:
	/** 
	 * Create a default corner detector
	 *    OpenCVCornerDetector(int corners_count = 400, double quality_level = 0.01, double min_distance = 8)
	 */
	PyrLKTwoViewPointTracker(void);

	/** Constructor with a CornerDetector
	 * assert pcd != NULL
	 * PyrLKTwoViewPointTracker is responsible for deleting the CornerDetector
	 */
	PyrLKTwoViewPointTracker(CornerDetector* pcd);

	~PyrLKTwoViewPointTracker(void);

public:
	/** Set PryLK point tracker parameters
	 */
	void setPryLKParams(int maxNumFeatures, int maxIteration, double accuracy, const CvSize& window) {
		this->maxNumFeatures = maxNumFeatures;
		this->maxIteration = maxIteration;
		this->accuracy = accuracy;
		this->window = window;
	}


public:
	void compute(const IplImage* im1, const IplImage* im2, std::vector<CvPoint2D32f>& x1s, std::vector<CvPoint2D32f>& x2s);

private:
	/**
	 * Default parameters:
	 *   maxNumFeatures = 400
	 *	 maxIteration = 20
	 *	 accuracy = 0.3
	 *	 window = cvSize(3,3)
	 */
	void initializeDefaultPryLKParams();

private:
	CornerDetector* pcd;

private:
	//parameters for pyramid LK tracking
	int maxNumFeatures;
	int maxIteration;
	double accuracy;
	CvSize window;

	float* errors;
	CvTermCriteria terminationCriteria;
	char* foundFeature;
};

}
#endif // FVISION_PYR_LK_TWO_VIEW_POINT_TRACKER_H_