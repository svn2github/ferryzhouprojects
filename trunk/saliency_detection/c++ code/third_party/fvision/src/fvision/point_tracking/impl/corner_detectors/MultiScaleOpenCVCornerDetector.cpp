#include "MultiScaleOpenCVCornerDetector.h"

namespace fvision {

void MultiScaleOpenCVCornerDetector::compute(const IplImage* src, std::vector<CvPoint2D32f>& corners) {

	std::vector<CvPoint2D32f> xs;
	pcd->compute(src, xs);

	IplImage* src2 = cvCreateImage(cvSize(src->width / 2, src->height / 2), src->depth, src->nChannels);
	cvResize(src, src2);
	cvSmooth(src2, src2);
	std::vector<CvPoint2D32f> xs2;
	pcd->compute(src2, xs2);

	corners.insert(corners.begin(), xs2.begin(), xs2.end());
	corners.insert(corners.end(), xs.begin(), xs.end());
}

}