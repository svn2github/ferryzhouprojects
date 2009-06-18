#include "drawing_PointCorrespondencesDrawer.h"

#include "Colors.h"
#include "drawing_PointDrawer.h"
#include "drawing_LineDrawer.h"
#include "fassert.h"
#include "geom.h"

namespace fvision {

PointCorrespondencesDrawer::PointCorrespondencesDrawer(void) {
	pointDrawer = new PointDrawer("+");
	lineDrawer = new LineDrawer(Colors::RED);
}

PointCorrespondencesDrawer::~PointCorrespondencesDrawer(void) {
	delete pointDrawer;
	delete lineDrawer;
}

IplImage* PointCorrespondencesDrawer::create(const IplImage* leftImage, const IplImage* rightImage, const PointPairs& pps) const {

	assert(fvision::isImageHeaderEqual(leftImage, rightImage));

	CvSize size = cvGetSize(leftImage);
	CvSize double_size = cvSize((int)(size.width * 1.5), size.height * 2);
	IplImage* newImage = cvCreateImage(double_size, 8, leftImage->nChannels);

	cvSetImageROI(newImage, cvRect(0, 0, size.width, size.height));
	cvCopy(leftImage, newImage);

	CvSize size2 = cvGetSize(rightImage);
	cvSetImageROI(newImage, cvRect(size.width / 2, size.height, size2.width, size2.height));
	cvCopy(rightImage, newImage);

	cvSetImageROI(newImage, cvRect(0, 0, double_size.width, double_size.height));
	CvPoint offset = cvPoint(size.width/2, size.height);
	for (int i = 0; i < (int)pps.size(); i++) {
		CvPoint p1 = cvPointFrom32f(pps[i].first);
		CvPoint p2 = cvPointFrom32f(pps[i].second) + offset;
		pointDrawer->draw(newImage, p1);
		pointDrawer->draw(newImage, p2);
		lineDrawer->draw(newImage, p1, p2);
	}
	return newImage;
}

IplImage* PointCorrespondencesDrawer::create(const IplImage* leftImage, const PointPairs& pps) const {
	IplImage* newImage = cvCreateImage(cvGetSize(leftImage), 8, leftImage->nChannels);
	cvCopy(leftImage, newImage);
	draw(newImage, pps);
	return newImage;
}

IplImage* PointCorrespondencesDrawer::create(const IplImage* leftImage, const IplImage* rightImage, const std::vector<CvPoint2D32f>& x1s, const std::vector<CvPoint2D32f>& x2s) const {
	PointPairs pps;
	composePointPairs(pps, x1s, x2s);
	return create(leftImage, rightImage, pps);
}

IplImage* PointCorrespondencesDrawer::create(const IplImage* leftImage, const std::vector<CvPoint2D32f>& x1s, const std::vector<CvPoint2D32f>& x2s) const {
	PointPairs pps;
	composePointPairs(pps, x1s, x2s);
	return create(leftImage, pps);
}

void PointCorrespondencesDrawer::setPointDrawer(const PointDrawer& pd) {
	*(this->pointDrawer) = pd;
}

void PointCorrespondencesDrawer::setLineDrawer(const LineDrawer& ld) {
	*(this->lineDrawer) = ld;
}

void PointCorrespondencesDrawer::draw(IplImage* leftImage, const PointPairs& pps) const {
	for (int i = 0; i < (int)pps.size(); i++) {
		pointDrawer->draw(leftImage, pps[i].first);
		lineDrawer->draw(leftImage, pps[i].first, pps[i].second);
	}
}

void PointCorrespondencesDrawer::draw(IplImage* leftImage, const std::vector<CvPoint2D32f>& x1s, const std::vector<CvPoint2D32f>& x2s) const {
	assert(x1s.size() == x2s.size());
	for (int i = 0; i < (int)x1s.size(); i++) {
		pointDrawer->draw(leftImage, x1s[i]);
		lineDrawer->draw(leftImage, x1s[i], x2s[i]);
	}
}

}