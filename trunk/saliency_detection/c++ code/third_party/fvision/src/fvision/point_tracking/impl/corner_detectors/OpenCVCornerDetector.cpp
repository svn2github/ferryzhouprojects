
#include "OpenCVCornerDetector.h"

using namespace std;

namespace fvision {

void OpenCVCornerDetector::compute(const IplImage* src, vector<CvPoint2D32f>& corners) {

	CvSize size = cvGetSize(src);
	IplImage* gray = cvCreateImage(size, 8, 1);
	
	if (src->nChannels == 3) {
		//gray = cvCreateImage(size, 8, 1);
		cvCvtColor(src, gray, CV_BGR2GRAY);
	} else cvCopy(src, gray);

	IplImage* eig_image = cvCreateImage(size, IPL_DEPTH_32F, 1);
	IplImage* temp_image = cvCreateImage(size, IPL_DEPTH_32F, 1);
	CvPoint2D32f* pcorners = new CvPoint2D32f[corners_count];
	//int corner_count = 1000;

	int temp_corners_count = corners_count;

	cvGoodFeaturesToTrack(gray, eig_image, temp_image, pcorners, &temp_corners_count, quality_level, min_distance);
	
	corners.clear();
	for (int i = 0; i < temp_corners_count; i++) {
		corners.push_back(pcorners[i]);
	}

	//if (clean) cleanCorners(corners);

	delete[] pcorners;
	cvReleaseImage(&gray);
	cvReleaseImage(&eig_image);
	cvReleaseImage(&temp_image);
}

}