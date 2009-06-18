#ifndef FVISION_SIFT_POINT_TRACKER_H_
#define FVISION_SIFT_POINT_TRACKER_H_

extern "C" {
#include "../sift/imgfeatures.h"
}

#include "../PointTracker.h"

namespace fvision {

class KdTree;

class SiftPointTracker : public PointTracker{
public:
	SiftPointTracker(void);
	~SiftPointTracker(void);

public:
	void addFrame(int frameId, const IplImage* frame);

	void setMinimalRatioOfNeighborDistances(double ratio);

	void setKeyFrameMinimalMatchRate(double rate);

	void setKeyFrameInterval(int interval);

private:
	void processOnFirstFrame(int frameId, const IplImage* frame);
	void processOnNewFrame(int frameId, const IplImage* frame);

	std::vector<int> computeRawCorrespondencesIndices(struct feature* features, int featureCount);

	void addFeaturesToResults(int frameId, const std::vector<int>& rawCorrespondences, struct feature* features);

	bool isAGoodMatch(struct feature* feat, const std::vector<feature*>& neighbors);
	
	//compute the index of neighbor in key frame features
	int computeIndexInKeyFrameFeatures(feature* neighbor);

	void updateLastKeyFrameMatched(const std::vector<int>& rawCorrespondences);

	double computeMatchRate(const std::vector<int>& rawCorrespondences);

	bool isKeyFrame(double matchRate);

	void deleteUnMatchedPointsInLastKeyFrame();

	void setUpNewKeyFrameData(struct feature* features, int featureCount, int frameId, const std::vector<int>& rawCorrespondences, KdTree* tree);

private:
	KdTree* kdTree;
	feature* keyFrameFeatures;
	int keyFrameFeatureCount;

	int keyFrameId;

	std::vector<int> keyFrameGlobalIndices;
	std::vector<int> keyFrameMatched;

	int processedFrameCount;

	int globalMaxFeatureId; //used to assign new feature's global id

	//params
	double NN_SQ_DIST_RATIO_THR; //threshold on squared ratio of distances between NN and 2nd NN 
	double keyFrameMininumMatchRate;
	int keyFrameInterval; //

};

}
#endif // FVISION_SIFT_POINT_TRACKER_H_