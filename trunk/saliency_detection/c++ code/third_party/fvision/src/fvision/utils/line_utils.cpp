#include "line_utils.h"

using namespace std;

namespace fvision {

// see http://en.wikipedia.org/wiki/Bresenham's_line_algorithm
std::vector<CvPoint> getLinePoints(int x0, int y0, int x1, int y1) {
	vector<CvPoint> points;
	bool steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}
	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}
	int deltax = x1 - x0;
	int deltay = abs(y1 - y0);
	int error = deltax / 2;
	int ystep;
	int y = y0;
	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}
	for (int x = x0; x <= x1; x++) {
		if (steep) {
			points.push_back(cvPoint(y, x));
		} else {
			points.push_back(cvPoint(x, y));
		}
		error = error - deltay;
		if (error < 0) {
			y += ystep;
			error += deltax;
		}
	}
	return points;
}


LinePointsIterator::LinePointsIterator(int x0, int y0, int x1, int y1) {
	points = getLinePoints(x0, y0, x1, y1);
	iter = points.begin();
}

bool LinePointsIterator::hasNext() const {
	return iter != points.end();
}

const CvPoint& LinePointsIterator::next() {
	iter++;
	return *(iter - 1);
}

LineSidePointsIterator::LineSidePointsIterator(int x0, int y0, int x1, int y1, bool rightSide, int rows) {
	points = getLineSidePoints(x0, y0, x1, y1, rightSide, rows);
	iter = points.begin();
}

bool LineSidePointsIterator::hasNext() const {
	return iter != points.end();
}

const CvPoint& LineSidePointsIterator::next() {
	iter++;
	return *(iter - 1);
}

std::vector<CvPoint> getLineSidePoints(int x0, int y0, int x1, int y1, bool rightSide, int rows) {
	vector<CvPoint> points = getLinePoints(x0, y0, x1, y1);
	vector<CvPoint> sidePoints;
	bool steep = abs(y1 - y0) > abs(x1 - x0);
	int delta = rightSide ? 1 : -1;
	if (steep) {
		for (int r = 1; r <= rows; r++) {
			int offset = delta * r;
			for (size_t i = 0; i < points.size(); i++) {
				CvPoint sidePoint = cvPoint(points[i].x + offset, points[i].y);
				sidePoints.push_back(sidePoint);
			}
		}
	} else {
		for (int r = 1; r <= rows; r++) {
			int offset = delta * r;
			for (size_t i = 0; i < points.size(); i++) {
				CvPoint sidePoint = cvPoint(points[i].x, points[i].y + offset);
				sidePoints.push_back(sidePoint);
			}
		}
	}
	return sidePoints;
}
}