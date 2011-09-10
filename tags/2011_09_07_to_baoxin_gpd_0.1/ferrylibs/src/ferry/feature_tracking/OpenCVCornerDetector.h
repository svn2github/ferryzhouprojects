#pragma once

#include <vector>

#include "highgui.h"
#include "cv.h"
#include "CornerDetector.h"

using namespace std;

namespace ferry {
	namespace feature_tracking {

class OpenCVCornerDetector : public CornerDetector
{
public:
	OpenCVCornerDetector(int corners_count, double quality_level, double min_distance) {
		this->corners_count = corners_count;
		this->quality_level = quality_level;
		this->min_distance = min_distance;
	}

public:
	void compute(IplImage* src, vector<CvPoint>& corners) {

		CvSize size = cvGetSize(src);
		IplImage* gray = cvCreateImage(size, 8, 1);
		
		if (src->nChannels == 3) {
			gray = cvCreateImage(size, 8, 1);
			cvCvtColor(src, gray, CV_BGR2GRAY);
		} else cvCopy(src, gray);

		IplImage* eig_image = cvCreateImage(size, IPL_DEPTH_32F, 1);
		IplImage* temp_image = cvCreateImage(size, IPL_DEPTH_32F, 1);
		CvPoint2D32f pcorners[1000];
		//int corner_count = 1000;

		int temp_corners_count = corners_count;

		cvGoodFeaturesToTrack(gray, eig_image, temp_image, pcorners, &temp_corners_count, quality_level, min_distance);
		
		corners.clear();
		for (int i = 0; i < temp_corners_count; i++) {
			corners.push_back(cvPointFrom32f(pcorners[i]));
		}

		//if (clean) cleanCorners(corners);

		cvReleaseImage(&gray);
		cvReleaseImage(&eig_image);
		cvReleaseImage(&temp_image);
	}


private:
	int corners_count;
	double quality_level;
	double min_distance;

};

	}
}