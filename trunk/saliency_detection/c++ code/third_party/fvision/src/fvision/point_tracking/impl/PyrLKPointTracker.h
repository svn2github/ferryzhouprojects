#ifndef FVISION_PYRLK_POINT_TRACKER_H_
#define FVISION_PYRLK_POINT_TRACKER_H_


#include "../PointTracker.h"
#include "CornerDetector.h"

namespace fvision {

/** Use Pyramid LK method to tracking points
 *
 * Parameters:
 *   pcd: CornerDetector
 *   keyFrameInterval: when to initialize a new key frame, default is 3
 *   keyFramePointsRatio: 
 *       another threshold for initialize a new key frame, 
 *       if the successfully tracked points is below a ratio, 
 *       default is 0.7
 */
class PyrLKPointTracker : public PointTracker {
public:
	/** Constructor
	 * @param pcd a corner detector
	 * 
	 * PryLKPointTracker is responsible for deleting pcd
	 */
	PyrLKPointTracker(CornerDetector* pcd);

public:
	~PyrLKPointTracker(void);

public:
	void addFrame(int frameId, const IplImage* frame);

public:
	void setKeyFrameInterval(int interval) { this->keyFrameInterval = interval; }

	void setKeyFramePointsRatio(int ratio) { this->keyFramePointsRatio = ratio; }

	/** Set PryLK point tracker parameters
	 * 
	 */
	void setPryLKParams(int maxNumFeatures, int maxIteration, double accuracy, const CvSize& window) {
		this->maxNumFeatures = maxNumFeatures;
		this->maxIteration = maxIteration;
		this->accuracy = accuracy;
		this->window = window;
	}

	void setMinimalNeighborDistance(double distance);

private:
	void initFrame(int frameId, IplImage* frame);

	//return true if p is near to any point of ps
	//if the distance of two points is less than MIN_NEAR_DISTANCE, they are near
	bool nearPoints(CvPoint2D32f p, const std::vector<CvPoint2D32f>& ps);

	bool isKeyFrame();

private:
	CornerDetector* pcd;

private:
	//parameters for pyramid LK tracking
	int maxNumFeatures;
	int maxIteration;
	double accuracy;
	float* errors;
	CvTermCriteria terminationCriteria;
	CvSize window;
	char* foundFeature;

	double MIN_NEAR_DISTANCE;

private:
	//status and buffers
	int processedFrameCount;
	int maxFeatureId;
	IplImage* lastKeyFrame;
	IplImage* lastKeyFramePyramid;
	IplImage* curFrame;
	IplImage* curFramePyramid;
	//found features
	CvPoint2D32f* lastKeyFrameFeatures;
	int* lastKeyFrameFeatureIds;
	size_t numFeatures;

	CvPoint2D32f* curFrameFeatures; //buffer for tracking
	std::vector<CvPoint2D32f> features2; //result of feature tracking
	std::vector<int> curFrameFeatureIds;

	bool keyFrame;

	CvSize imSize;

	int keyFrameInterval; //default is 3. should be automatically determined by translation length
	double keyFramePointsRatio; //default 0.7
};

}
#endif // FVISION_PYRLK_POINT_TRACKER_H_