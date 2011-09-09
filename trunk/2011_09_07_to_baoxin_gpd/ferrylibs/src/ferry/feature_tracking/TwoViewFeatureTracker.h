#pragma once

#include <cassert>

#include "SimpleWindowPOFM.h"
#include "OpenCVCornerDetector.h"

//#include "DrawingUtil.h"

//using namespace ferry::cv_geometry;

namespace ferry {
	namespace feature_tracking {

/**
 * Given two images, corner detector and feature matcher, compute point correspondences
 *
 */
class TwoViewFeatureTracker {

public:
	TwoViewFeatureTracker(CornerDetector* pcd = NULL, PointsOutFeatureMatcher* ppofm = NULL){
		this->pcd = (pcd != NULL) ? pcd : new OpenCVCornerDetector(400, 0.05, 8);
		this->ppofm = (ppofm != NULL) ? ppofm : new SimpleWindowPOFM(new SimpleWindowSSDFDC(5, 3, 20), 80);
	}

public:
	/**
	 * Input: two images
	 * Output: point correspondences
	 */
	void compute(IplImage* im1, IplImage* im2, vector<CvPoint>& x1s, vector<CvPoint>& x2s) {
		CvSize size1 = cvGetSize(im1);
		CvSize size2 = cvGetSize(im2);
		assert(size1.height == size2.height && size1.width == size2.width && im1->nChannels == im2->nChannels);
		
		bool color = (im1->nChannels == 3);
		IplImage* g_im1 = NULL;
		IplImage* g_im2 = NULL;
		if (color) {
			g_im1 = cvCreateImage(size1, 8, 1);
			g_im2 = cvCreateImage(size1, 8, 1);
			cvCvtColor(im1, g_im1, CV_BGR2GRAY);
			cvCvtColor(im2, g_im2, CV_BGR2GRAY);
		} else {
			g_im1 = im1;
			g_im2 = im2;
		}

		vector<CvPoint> corners1;
		vector<CvPoint> corners2;
		pcd->compute(im1, corners1);
		pcd->compute(im2, corners2);

		//::drawCornersImage("corners1", im1, corners1);
		//::drawCornersImage("corners2", im2, corners2);

		ppofm->compute(g_im1, g_im2, corners1, corners2, x1s, x2s);

		if (color) {
			cvReleaseImage(&g_im1);
			cvReleaseImage(&g_im2);
		}
	}

private:
	CornerDetector* pcd;
	PointsOutFeatureMatcher* ppofm;
};

	}
}