#include "SiftPointTracker.h"

extern "C" {
#include "../sift/sift.h"
#include "../sift/imgfeatures.h"
#include "../sift/kdtree.h"
#include "../sift/utils.h"
}

#include "../sift/KdTreeWrapper.h"

#include <iostream>

using namespace std;

namespace fvision {

SiftPointTracker::SiftPointTracker(void) {
	kdTree = NULL;
	processedFrameCount = 0;
	keyFrameFeatures = NULL;
	keyFrameFeatureCount = 0;

	keyFrameMininumMatchRate = 0.4;
	keyFrameInterval = 1;

	globalMaxFeatureId = 0;
}

SiftPointTracker::~SiftPointTracker(void) {
	if (kdTree != NULL) delete kdTree;
	if (keyFrameFeatures != NULL) free(keyFrameFeatures);
}

void SiftPointTracker::setMinimalRatioOfNeighborDistances(double ratio) {
	assert (ratio > 0 && ratio < 1); 
	this->NN_SQ_DIST_RATIO_THR = ratio;
}

void SiftPointTracker::setKeyFrameMinimalMatchRate(double rate) {
	assert (rate > 0 && rate < 1); 
	this->keyFrameMininumMatchRate = rate;
}

void SiftPointTracker::setKeyFrameInterval(int interval) {
	assert (interval > 0);
	this->keyFrameInterval = interval;
}

/**

For the first frame:
  extract features
  create global indices
  create kd_tree

For each new frame:
  extract features
  compute correspondences vector cv such that cv[i] is the raw index of the last key frame feature for the i-th raw feature in the current frame
  assign global index to each element of cv based on global indices of last key frame features
  append points along with global indices to the results
  
  if this is a key frame
    set keyframe
    update global indices
	replace kd_tree with current frame features

*/
void SiftPointTracker::addFrame(int frameId, const IplImage* frame) {
	if (processedFrameCount == 0) { //first frame
		processOnFirstFrame(frameId, frame);
	} else {
		processOnNewFrame(frameId, frame);
	}
	processedFrameCount++;
}

void SiftPointTracker::processOnFirstFrame(int frameId, const IplImage* frame) {
	keyFrameId = frameId;

	keyFrameFeatureCount = sift_features((IplImage*)frame, &keyFrameFeatures);

	keyFrameGlobalIndices.resize(keyFrameFeatureCount);

	//the content of keyFrameFeatures changed! so this expression have to be put before using its values
	kdTree = new KdTree(keyFrameFeatures, keyFrameFeatureCount); 

	for (int i = 0; i < keyFrameFeatureCount; i++) {
		CvPoint2D32f p = cvPoint2D32f(keyFrameFeatures[i].x, keyFrameFeatures[i].y);
		ptdb->insertItem(frameId, i, p);
		keyFrameGlobalIndices[i] = i;
	}
	globalMaxFeatureId = keyFrameFeatureCount;

	keyFrameMatched.resize(keyFrameFeatureCount, -1);
}

void SiftPointTracker::processOnNewFrame(int frameId, const IplImage* frame) {

	struct feature* features;
	int featureCount = sift_features((IplImage*)frame, &features);
	cout<<frameId<<" ferature count: "<<featureCount<<endl;

	KdTree* tree = new KdTree(features, featureCount);

	vector<int> rawCorrespondences = computeRawCorrespondencesIndices(features, featureCount);

	addFeaturesToResults(frameId, rawCorrespondences, features);

	updateLastKeyFrameMatched(rawCorrespondences);

	double matchRate = computeMatchRate(rawCorrespondences);
	cout<<"match rate: "<<matchRate<<endl;

	if (isKeyFrame(matchRate)) {
		deleteUnMatchedPointsInLastKeyFrame();
		setUpNewKeyFrameData(features, featureCount, frameId, rawCorrespondences, tree);
	} else {
		free(features);
		delete tree;
	}
}

std::vector<int> SiftPointTracker::computeRawCorrespondencesIndices(struct feature* features, int featureCount) {

	vector<int> correspondenceIndices(featureCount);

	for(int i = 0; i < featureCount; i++ ) {
		struct feature* feat = features + i;
		vector<feature*> neighbors = kdTree->getKNearestNeighbors(feat, 2);
		if (isAGoodMatch(feat, neighbors)) {
			int index = (int)(neighbors[0] - keyFrameFeatures);
			//cout<<keyFrameGlobalIndices[index]<<" -> "<<i<<" == "<<neighbors[0]->x<<", "<<neighbors[0]->y<<" -> "<<feat->x<<", "<<feat->y;
			//cout<<" <- "<<keyFrameFeatures[index].x<<", "<<keyFrameFeatures[index].y;
			//CvPoint2D32f p = ptdb->getObjectByFrameIdAndObjectId(keyFrameId, keyFrameGlobalIndices[index]);
			//cout<<" <- "<<p.x<<", "<<p.y<<endl;
			correspondenceIndices[i] = index;
		} else {
			correspondenceIndices[i] = -1; //can not find
		}
	}

	return correspondenceIndices;
}

int SiftPointTracker::computeIndexInKeyFrameFeatures(feature* neighbor) {
	for (int i = 0; i < keyFrameFeatureCount; i++) {
		if ((keyFrameFeatures + i) == neighbor) return i;
	}
	assert(false); //can not go to here!
	return -1;
}

bool SiftPointTracker::isAGoodMatch(struct feature* feat, const vector<feature*>& neighbors) {
	if (neighbors.size() < 2) return false;
	double d0 = descr_dist_sq( feat, neighbors[0] );
	double d1 = descr_dist_sq( feat, neighbors[1] );
	if (d0 < d1 * NN_SQ_DIST_RATIO_THR) return true;
	else return false;
}

void SiftPointTracker::updateLastKeyFrameMatched(const std::vector<int>& rawCorrespondences) {
	for (size_t i = 0; i < rawCorrespondences.size(); i++) {
		int rawCorrespondence = rawCorrespondences[i];
		if (rawCorrespondence != -1) {
			keyFrameMatched[rawCorrespondence] = 1;
		}
	}
}

void SiftPointTracker::addFeaturesToResults(int frameId, const std::vector<int>& rawCorrespondences, struct feature* features) {
	for (size_t i = 0; i < rawCorrespondences.size(); i++) {
		int rawCorrespondence = rawCorrespondences[i];
		if (rawCorrespondence != -1) {
			int globalIndex = keyFrameGlobalIndices[rawCorrespondence];
			CvPoint2D32f p = cvPoint2D32f(features[i].x, features[i].y);
			ptdb->insertItem(frameId, globalIndex, p);
		}
	}
}

double SiftPointTracker::computeMatchRate(const std::vector<int>& rawCorrespondences) {
	if (rawCorrespondences.size() == 0) return 0;
	size_t negativeIndicesCount = 0;
	for (size_t i = 0; i < rawCorrespondences.size(); i++) {
		if (rawCorrespondences[i] < 0) negativeIndicesCount++;
	}
	return (double)negativeIndicesCount / rawCorrespondences.size();
}

bool SiftPointTracker::isKeyFrame(double matchRate) {
	return (matchRate < keyFrameMininumMatchRate) || ((processedFrameCount % keyFrameInterval) == 0);
}

void SiftPointTracker::deleteUnMatchedPointsInLastKeyFrame() {
	cout<<keyFrameId<<" now has "<<ptdb->getAllObjectIdsInFrame(keyFrameId).size()<<" elements in the beginning"<<endl;
	for (size_t i = 0; i < keyFrameMatched.size(); i++) {
		if (keyFrameMatched[i] < 0) {
			bool ret = ptdb->deleteItem(keyFrameId, keyFrameGlobalIndices[i]);
			assert (ret);
		}
	}
	cout<<keyFrameId<<" now has "<<ptdb->getAllObjectIdsInFrame(keyFrameId).size()<<" elements after delete unmatched points"<<endl;
}

void SiftPointTracker::setUpNewKeyFrameData(struct feature* features, int featureCount, int frameId, const vector<int>& rawCorrespondences, KdTree* tree) {

	keyFrameId = frameId;

	vector<int> newKeyFrameGlobalIndices(rawCorrespondences.size());
	keyFrameMatched.resize(rawCorrespondences.size(), -1);
	for (size_t i = 0; i < rawCorrespondences.size(); i++) {
		int rawCorrespondence = rawCorrespondences[i];
		if (rawCorrespondence != -1) {
			newKeyFrameGlobalIndices[i] = keyFrameGlobalIndices[rawCorrespondence];
			keyFrameMatched[i] = 1; //matched
		} else {
			newKeyFrameGlobalIndices[i] = globalMaxFeatureId++;
			CvPoint2D32f p = cvPoint2D32f(features[i].x, features[i].y);
			ptdb->insertItem(frameId, newKeyFrameGlobalIndices[i], p);
		}
	}
	keyFrameGlobalIndices = newKeyFrameGlobalIndices;

	free(keyFrameFeatures);
	keyFrameFeatures = features;
	keyFrameFeatureCount = featureCount;
	delete kdTree;
	kdTree = tree;

}
}