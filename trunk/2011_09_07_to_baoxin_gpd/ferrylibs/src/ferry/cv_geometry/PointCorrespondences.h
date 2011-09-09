#pragma once

#include "BasicUtil.h"

namespace ferry {
	namespace cv_geometry {

class PointCorrespondences
{
public:
	PointCorrespondences() {
	}

	PointCorrespondences(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s) {
		this->x1s = x1s;
		this->x2s = x2s;
	}

	PointCorrespondences(const vector<CvPoint>& x1s, const vector<CvPoint>& x2s) {
		this->x1s = cvPointsTo32fs(x1s);
		this->x2s = cvPointsTo32fs(x2s);
	}

	~PointCorrespondences(void) {}

public:
	void clear() {
		x1s.clear();
		x2s.clear();
	}

	void reset(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s) {
		this->x1s = x1s;
		this->x2s = x2s;
	}

	void reset(const vector<CvPoint>& x1s, const vector<CvPoint>& x2s) {
		this->x1s = cvPointsTo32fs(x1s);
		this->x2s = cvPointsTo32fs(x2s);
	}

public:
	vector<CvPoint2D32f> x1s;
	vector<CvPoint2D32f> x2s;
};

	}
}
