#include "PyrLKTwoViewPointTracker.h"

#include "corner_detectors/OpenCVCornerDetector.h"

#include <cassert>

using namespace std;

namespace fvision {

PyrLKTwoViewPointTracker::PyrLKTwoViewPointTracker(void) {
	pcd = new OpenCVCornerDetector(400, 0.01, 8);
	initializeDefaultPryLKParams();
}

PyrLKTwoViewPointTracker::PyrLKTwoViewPointTracker(CornerDetector* pcd) {
	assert(pcd != NULL);
	this->pcd = pcd;
	initializeDefaultPryLKParams();
}

PyrLKTwoViewPointTracker::~PyrLKTwoViewPointTracker(void) {
	delete pcd;
}

void PyrLKTwoViewPointTracker::initializeDefaultPryLKParams() {
	maxNumFeatures = 400;
	maxIteration = 20;
	accuracy = 0.3;
	window = cvSize(3,3);
}


void PyrLKTwoViewPointTracker::compute(const IplImage* im1, const IplImage* im2, vector<CvPoint2D32f>& x1s, vector<CvPoint2D32f>& x2s) {

	x1s.clear();
	x2s.clear();

	vector<CvPoint2D32f> corners1;
	pcd->compute(im1, corners1);

	terminationCriteria = cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, maxIteration, accuracy);
	
	//get gray image
	bool color = (im1->nChannels == 3);
	IplImage* gim1 = (IplImage*)im1;
	IplImage* gim2 = (IplImage*)im2;
	CvSize size = cvGetSize(im1);
	if (color) {
		gim1 = cvCreateImage(size, 8, 1);
		cvCvtColor(im1, gim1, CV_BGR2GRAY);
		gim2 = cvCreateImage(size, 8, 1);
		cvCvtColor(im2, gim2, CV_BGR2GRAY);
	}

	IplImage* pyramid1 = cvCreateImage(size, IPL_DEPTH_8U, 1);
	IplImage* pyramid2 = cvCreateImage(size, IPL_DEPTH_8U, 1);

	int number_of_features = (int)(corners1.size());
	CvPoint2D32f* frame1_features = new CvPoint2D32f[number_of_features];
	CvPoint2D32f* frame2_features = new CvPoint2D32f[number_of_features];
	for (int i = 0; i < number_of_features; i++) {
		frame1_features[i] = corners1[i];
	}

	char* optical_flow_found_feature = new char[number_of_features];
	float* optical_flow_feature_error = new float[number_of_features];

	cvCalcOpticalFlowPyrLK(
		gim1, gim2, 
		pyramid1, pyramid2, 
		frame1_features, frame2_features, 
		number_of_features, 
		window, 
		5, //level
		optical_flow_found_feature, 
		optical_flow_feature_error, 
		terminationCriteria, 
		0 
	);

	for(int i = 0; i < number_of_features; i++) {
		/* If Pyramidal Lucas Kanade didn't really find the feature, skip it. */
		if ( optical_flow_found_feature[i] == 0 )	continue;
	
		x1s.push_back(frame1_features[i]);
		x2s.push_back(frame2_features[i]);
	}

	if (color) {
		cvReleaseImage(&gim1);
		cvReleaseImage(&gim2);
	}

	cvReleaseImage(&pyramid1);
	cvReleaseImage(&pyramid2);

	delete[] frame1_features;
	delete[] frame2_features;
	delete[] optical_flow_found_feature;
	delete[] optical_flow_feature_error;


}

}