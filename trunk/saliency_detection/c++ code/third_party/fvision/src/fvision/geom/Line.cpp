#include "Line.h"

#include "rotation.h"

#include <fvision/utils.h>

namespace fvision {

Line::Line(const CvPoint2D32f& pt1, const CvPoint2D32f& pt2) {
	initWithPoints(pt1, pt2);
}

Line::Line(const CvPoint& pt1, const CvPoint& pt2) {
	initWithPoints(cvPointTo32f(pt1), cvPointTo32f(pt2));
}

Line::Line(float x1, float y1, float x2, float y2) {
	initWithPoints(cvPoint2D32f(x1, y1), cvPoint2D32f(x2, y2));
}

Line::Line() {
	initWithPoints(cvPoint2D32f(0, 0), cvPoint2D32f(0, 0));
}

Line::Line(const Line& rhs) {
	initWithPoints(rhs.p1, rhs.p2);
}

Line::~Line(void) {
	cvReleaseMat(&mat);
}

void Line::setPoints(const CvPoint2D32f& pt1, const CvPoint2D32f& pt2) {
	this->p1 = pt1;
	this->p2 = pt2;
	updateMat();
}

void Line::updateMat() {
	cvmSet(mat, 0, 0, p1.y - p2.y);
	cvmSet(mat, 1, 0, p2.x - p1.x);
	cvmSet(mat, 2, 0, p1.x * p2.y - p1.y * p2.x);
}

void Line::initWithPoints(const CvPoint2D32f& pt1, const CvPoint2D32f& pt2) {
	this->p1 = pt1;
	this->p2 = pt2;
	mat = cvCreateMat(3, 1, CV_64FC1);
	updateMat();
}

Line& Line::operator=(const Line& rhs) {
	if (&rhs == this) return *this;
	setPoints(rhs.p1, rhs.p2);
	return *this;
}

double Line::getYAngle() const {
	return normalizeTheta(atan2(p2.x - p1.x, p2.y - p1.y));
}

double Line::getLength() const {
	return pointDistance(p1, p2);
}

std::ostream& operator<< (std::ostream& os, const Line& line) {
	os<<line.getPoint1().x<<" "<<line.getPoint1().y<<" "<<line.getPoint2().x<<" "<<line.getPoint2().y;
	return os;
}

std::istream& operator>> (std::istream& is, Line& line) {
	CvPoint2D32f p1, p2;
	is>>p1.x>>p1.y>>p2.x>>p2.y;
	line.setPoints(p1, p2);
	return is;
}
}