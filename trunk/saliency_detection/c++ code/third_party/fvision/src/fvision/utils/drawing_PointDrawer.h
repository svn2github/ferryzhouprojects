#pragma once

#include <cv.h>

#include <string>

namespace fvision {

class PointDrawer {
public:
	PointDrawer(const std::string& pattern = "o",  CvScalar color = CV_RGB(0, 255, 0), int size = 5, int thickness = 1, int line_type = 8) {
		this->pattern = pattern;
		this->color = color;
		this->size = size;
		this->thickness = thickness;
		this->line_type = line_type;
	}

public:
	void draw(IplImage* src, const CvPoint& pt) const ;
	void draw(IplImage* src, const CvPoint2D32f& pt) const ;

public:
	std::string pattern;
	CvScalar color;
	int size;
	int thickness;
	int line_type;
};

}