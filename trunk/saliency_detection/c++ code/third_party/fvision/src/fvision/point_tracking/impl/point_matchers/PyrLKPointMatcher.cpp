#include "PyrLKPointMatcher.h"

using namespace std;

namespace fvision {

void PyrLKPointMatcher::match(
	const IplImage* im1, const IplImage* im2, 
	const vector<CvPoint>& corners1, const vector<CvPoint>& corners2, 
	vector<CvPoint>& x1s, vector<CvPoint>& x2s
	) 
{
	CvSize size = cvGetSize(im1);

	//get gray image
	bool color = (im1->nChannels == 3);
	IplImage* gim1 = (IplImage*)im1;
	IplImage* gim2 = (IplImage*)im2;
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
		frame1_features[i] = cvPointTo32f(corners1[i]);
	}

	CvSize optical_flow_window = cvSize(3,3);

	char* optical_flow_found_feature = new char[number_of_features];
	float* optical_flow_feature_error = new float[number_of_features];

	const int MAX_ITERATION = 20;
	const double ACCURACY = 0.3;
	CvTermCriteria optical_flow_termination_criteria
		= cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, MAX_ITERATION, ACCURACY);

	cvCalcOpticalFlowPyrLK(
		gim1, gim2, 
		pyramid1, pyramid2, 
		frame1_features, frame2_features, 
		number_of_features, 
		optical_flow_window, 
		5, //level
		optical_flow_found_feature, 
		optical_flow_feature_error, 
		optical_flow_termination_criteria, 
		0 
	);

	for(int i = 0; i < number_of_features; i++) {
		/* If Pyramidal Lucas Kanade didn't really find the feature, skip it. */
		if ( optical_flow_found_feature[i] == 0 )	continue;
	
		x1s.push_back(cvPointFrom32f(frame1_features[i]));
		x2s.push_back(cvPointFrom32f(frame2_features[i]));
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