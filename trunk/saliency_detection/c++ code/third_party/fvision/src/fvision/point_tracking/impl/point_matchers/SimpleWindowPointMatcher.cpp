#include "SimpleWindowPointMatcher.h"

#include "../../matching_utils.h"

#include <algorithm>

using namespace std;

namespace fvision {

void SimpleWindowPointMatcher::match(const IplImage* im1, 
									 const IplImage* im2, 
									 const vector<CvPoint>& corners1, 
									 const vector<CvPoint>& corners2, 
									 vector<CvPoint>& x1s, 
									 vector<CvPoint>& x2s) 
{

	IplImage* nim1 = NULL;
	IplImage* nim2 = NULL;

	if (im1->nChannels == 1) {
		nim1 = cvCloneImage(im1);
		nim2 = cvCloneImage(im2);
	} else {
		CvSize size = cvGetSize(im1);
		if (type == 0) {
			nim1 = cvCreateImage(size, 8, 1);
			nim2 = cvCreateImage(size, 8, 1);
			cvCvtColor(im1, nim1, CV_BGR2GRAY);
			cvCvtColor(im2, nim2, CV_BGR2GRAY);
		} else if (type == 1) {
			nim1 = cvCloneImage(im1);
			nim2 = cvCloneImage(im2);
		} else if (type == 2) {
			nim1 = cvCreateImage(size, 8, 3);
			nim2 = cvCreateImage(size, 8, 3);
			cvCvtColor(im1, nim1, CV_BGR2HSV);
			cvCvtColor(im2, nim2, CV_BGR2HSV);
		} else if (type == 3) {
			nim1 = cvCreateImage(size, 8, 3);
			nim2 = cvCreateImage(size, 8, 3);
			//rgb = RGB/(R+G+B) * 255
		} else {
			nim1 = cvCloneImage(im1);
			nim2 = cvCloneImage(im2);
		}
	}

	for (size_t i = 0; i < corners1.size(); i++) {
		CvPoint p2 = search(nim1, nim2, corners1[i], corners2);
		if (p2.x == -1) { //can't find
		} else {
			x1s.push_back(corners1[i]);
			x2s.push_back(p2);
		}
	}

	cvReleaseImage(&nim1);
	cvReleaseImage(&nim2);
}

//
CvPoint SimpleWindowPointMatcher::search(const IplImage* im1, 
										 const IplImage* im2,
										 const CvPoint& p1,
										 const vector<CvPoint>& corners2) {

	vector<CvPoint> cps = filterPointsByDistance(p1, corners2, maxDistance);
	if (cps.size() == 0) return cvPoint(-1, -1);

	vector<float> diffs;
	for (size_t i = 0; i < cps.size(); i++) {
		diffs.push_back(windowMatch(im1, im2, p1, cps[i], halfWindowSize, type));
	}
	vector<float>::iterator iter = min_element(diffs.begin(), diffs.end());
	if (*iter > threshold) return cvPoint(-1, -1);
	return cps[iter - diffs.begin()];
}

}