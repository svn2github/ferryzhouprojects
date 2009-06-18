#include "BlockCornerDetector.h"

using namespace std;

namespace fvision {

void BlockCornerDetector::compute(const IplImage* src, vector<CvPoint2D32f>& corners) {
	OpenCVCornerDetector* pcd = new OpenCVCornerDetector(corners_count, quality_level, min_distance);
	CvSize size = cvGetSize(src);
	int xsize = size.width / nxslice;
	int ysize = size.height / nyslice;
	IplImage* nsrc = cvCloneImage(src);
	for (int i = 0; i < nxslice; i++) {
		for (int j = 0; j < nyslice; j++) {
			vector<CvPoint2D32f> block_corners;
			cvSetImageROI(nsrc, cvRect(i * xsize, j * ysize, xsize, ysize));
			pcd->compute(nsrc, block_corners);
			addCorners(block_corners, i * xsize, j * ysize, corners);
		}
	}
	cvReleaseImage(&nsrc);
	delete pcd;
}

void BlockCornerDetector::addCorners(const vector<CvPoint2D32f>& block_corners, int x, int y, vector<CvPoint2D32f>& corners) {
	for (int i = 0; i < (int)block_corners.size(); i++) {
		corners.push_back(cvPoint2D32f(block_corners[i].x + x, block_corners[i].y + y));
	}
}

}