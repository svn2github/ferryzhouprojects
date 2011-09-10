#pragma once

#include <cv.h>
#include <ferry/cv_geometry/PointCorrespondences.h>

using namespace ferry::cv_geometry;

class NHGroundDetector
{
public:
	NHGroundDetector(CvMat* K) {
		this->K = K;
	}

	~NHGroundDetector(void) {}

public:
	void compute(IplImage* im1, IplImage* im2) {
	}

	PointCorrespondences getGroundPoints() {
	}

	PointCorrespondences getObstaclePoints() {
	}

private:
	CvMat* K;

	PointCorrespondences gps;
	PointCorrespondences ops;

};
