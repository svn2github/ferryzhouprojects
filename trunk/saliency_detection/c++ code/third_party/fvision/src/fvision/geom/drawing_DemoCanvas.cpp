#include "drawing_DemoCanvas.h"

#include <fvision/utils/drawing_PointDrawer.h>
#include <fvision/utils/drawing_LineDrawer.h>
#include <fvision/utils/drawing_TextDrawer.h>
#include <fvision/utils/cvmat_utils.h>
#include <fvision/utils/drawing.h>
#include <fvision/utils/geom.h>

#include "camera_utils.h"
#include "PointTransformerFactory.h"

using namespace std;

namespace fvision {

DemoCanvas::DemoCanvas(void) {
	this->im = NULL;
	this->lineBuffer = cvCreateMat(3, 1, CV_64FC1);
	this->lineHomography = cvCreateMat(3, 3, CV_64FC1);
	K = NULL;
	bgColor = CV_RGB(0, 0, 0);
}

DemoCanvas::~DemoCanvas(void) {
	cvReleaseImage(&im);
	cvReleaseMat(&lineBuffer);
	cvReleaseMat(&lineHomography);
	if (K != NULL) cvReleaseMat(&K);
}

void DemoCanvas::setUp(const CvSize& imSize, float scale) {
	this->imSize = imSize;
	this->imCenter = cvPoint2D32f(imSize.width / 2, imSize.height / 2);
	this->scale = scale;

	if (K != NULL) cvReleaseMat(&K);
	this->K = getKFromFocalLengthAndImageSize(scale, imSize);

	PointTransformerFactory factory;
	this->pointTransformer = factory.createKDeNormalizer(K);
	CvMat* invK = inv(K);
	cvTranspose(invK, lineHomography);
	cvReleaseMat(&invK);

	if (im != NULL) cvReleaseImage(&im);
	this->im = cvCreateImage(imSize, 8, 3);

	clean();
}

void DemoCanvas::clean() {
	//cvZero(im);
	cvSet(im, bgColor);
}

CvPoint2D32f DemoCanvas::transformPoint(const CvPoint2D32f p) {
	return pointTransformer->transform(p);	
}

void DemoCanvas::drawPoint(const CvPoint2D32f& p, const PointDrawer& pd) {
	pd.draw(im, transformPoint(p));
}

void DemoCanvas::drawLine(const CvPoint2D32f& p1, const CvPoint2D32f& p2, const LineDrawer& ld) {
	ld.draw(im, transformPoint(p1), transformPoint(p2));
}

void DemoCanvas::drawLine(const CvMat* line, const LineDrawer& ld) {
	cvMatMul(lineHomography, line, lineBuffer);
	fvision::drawLine(im, lineBuffer, ld);
}

void DemoCanvas::drawPose(const CvPoint2D32f& camPosition, double camAngle, const PointDrawer& pd, const LineDrawer& ld, float arrowLength) {	
	drawPoint(camPosition, pd);
	CvPoint2D32f p = cvPoint2D32f(0, arrowLength / cvmGet(K, 0, 0));
	CvPoint2D32f arrowEnd = camPosition + rotatePoint(p, -camAngle);
	drawLine(camPosition, arrowEnd, ld);
}

void DemoCanvas::drawPoints(const std::vector<CvPoint2D32f>& points, const PointDrawer& pd) {
	for (size_t i = 0; i < points.size(); i++) {
		drawPoint(points[i], pd);
	}
}

void DemoCanvas::drawText(const CvPoint2D32f& position, const CvPoint2D32f& imOffset, const TextDrawer& td, const string& text) {
	td.draw(im, transformPoint(position) + imOffset, text.c_str());
}

void DemoCanvas::setBackgroundColor(const CvScalar& color) {
	this->bgColor = color;
}
}