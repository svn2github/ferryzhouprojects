#include "PyrLKPointTracker.h"

#include <iostream>

using namespace std;

namespace fvision {

PyrLKPointTracker::PyrLKPointTracker(CornerDetector* pcd) {

	this->pcd = pcd;

	MIN_NEAR_DISTANCE = 10;//8;

	maxNumFeatures = 500;//400;
	maxIteration = 20;
	accuracy = 0.3;
	terminationCriteria = cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, maxIteration, accuracy);
	window = cvSize(3,3);

	processedFrameCount = 0;
	maxFeatureId = 0;
	
	lastKeyFrame = NULL;
	lastKeyFramePyramid = NULL;
	curFrame = NULL;

	lastKeyFrameFeatures = new CvPoint2D32f[maxNumFeatures];
	curFrameFeatures = new CvPoint2D32f[maxNumFeatures];
	errors = new float[maxNumFeatures];
	foundFeature = new char[maxNumFeatures];

	lastKeyFrameFeatureIds = new int[maxNumFeatures];

	keyFrame = false;

	keyFrameInterval = 3;
	keyFramePointsRatio = 0.7;
}

PyrLKPointTracker::~PyrLKPointTracker(void) {
	delete pcd;
}

//lastKeyFrame -> frame
//track
//if valid tracked points percentage is less than 70%
//   set frame as newKeyFrame, new feature points merge tracked points as feature points
//   update db with new id points
//else update db with id points
void PyrLKPointTracker::addFrame(int frameId, const IplImage* frame) {
	IplImage* gframe = (IplImage*)frame;
	if (frame->nChannels != 1) {
		gframe = cvCreateImage(cvGetSize(frame), frame->depth, 1);
		cvCvtColor(frame, gframe, CV_BGR2GRAY);
	}

	if (processedFrameCount == 0) { //first frame
		initFrame(frameId, gframe);
		processedFrameCount++;
		if (frame->nChannels != 1) cvReleaseImage(&gframe);
		return;
	}

	//not first frame and last key frame is set

	//track
	cvCalcOpticalFlowPyrLK(
		lastKeyFrame, gframe, 
		lastKeyFramePyramid,curFramePyramid, 
		lastKeyFrameFeatures, curFrameFeatures, 
		(int)numFeatures, 
		window, 
		5, //level
		foundFeature, 
		errors, 
		terminationCriteria, 
		0 
	);

	//update ptdb
	features2.clear();
	curFrameFeatureIds.clear();
	for (size_t i = 0; i < numFeatures; i++) {
		if (foundFeature[i] == 0) continue;
		ptdb->insertItem(frameId, lastKeyFrameFeatureIds[i], curFrameFeatures[i]);
		features2.push_back(curFrameFeatures[i]);
		curFrameFeatureIds.push_back(lastKeyFrameFeatureIds[i]);
		//cout<<i<<" "<<lastKeyFrameFeatures[i]<<" -> "<<curFrameFeatures[i]<<endl;
	}
	keyFrame = false;

	if (isKeyFrame()) { //new keyframe
		//set up new key frame status
		keyFrame = true;
		cvCopy(gframe, lastKeyFrame);
		cvCopy(curFramePyramid, lastKeyFramePyramid);
		for (size_t i = 0; i < features2.size(); i++) {
			lastKeyFrameFeatures[i] = features2[i];
			lastKeyFrameFeatureIds[i] = curFrameFeatureIds[i];
		}
		numFeatures = features2.size();

		//find new features
		vector<CvPoint2D32f> xs;
		pcd->compute(gframe, xs);

		//merge
		//if new feature is already near any point in the old feature, discard it
		for (size_t i = 0; i < xs.size() && (int)numFeatures < maxNumFeatures; i++) {
			if (!nearPoints(xs[i], features2)) { //new!
				lastKeyFrameFeatures[numFeatures] = xs[i];
				lastKeyFrameFeatureIds[numFeatures] = maxFeatureId;	
				ptdb->insertItem(frameId, maxFeatureId, xs[i]);
				maxFeatureId++;
				numFeatures++;
			}
		}
		
	}

	processedFrameCount++;

	if (frame->nChannels != 1) cvReleaseImage(&gframe);

}

//compute feature points
//update ptdb
//setup last key frame status and buffer
void PyrLKPointTracker::initFrame(int frameId, IplImage* frame) {

	vector<CvPoint2D32f> xs;
	pcd->compute(frame, xs);

	for (size_t i = 0; i < xs.size(); i++) {
		ptdb->insertItem(frameId, maxFeatureId, xs[i]);
		maxFeatureId++;		
	}

	lastKeyFrame = cvCloneImage(frame);
	for (int i = 0; i < (int)xs.size(); i++) {
		lastKeyFrameFeatures[i] = xs[i];
		lastKeyFrameFeatureIds[i] = i;
	}
	numFeatures = xs.size();

	keyFrame = true;

	imSize = cvGetSize(frame);
	lastKeyFramePyramid = cvCreateImage(imSize, IPL_DEPTH_8U, 1);
	curFramePyramid = cvCreateImage(imSize, IPL_DEPTH_8U, 1);

}

double pt_pointDistance(const CvPoint2D32f& p1, const CvPoint2D32f& p2) {
	float dx = p1.x - p2.x;
	float dy = p1.y - p2.y;
	return sqrt((dx * dx + dy * dy));			
}

bool PyrLKPointTracker::nearPoints(CvPoint2D32f p, const vector<CvPoint2D32f>& ps) {
	for (size_t i = 0; i < ps.size(); i++) {
		if (pt_pointDistance(p, ps[i]) < MIN_NEAR_DISTANCE) return true;
	}
	return false;
}

bool PyrLKPointTracker::isKeyFrame() {
	return ((float)features2.size() / numFeatures < keyFramePointsRatio || processedFrameCount % keyFrameInterval == 0);
}

void PyrLKPointTracker::setMinimalNeighborDistance(double distance) {
	assert (distance > 0);
	this->MIN_NEAR_DISTANCE = distance;
}
}