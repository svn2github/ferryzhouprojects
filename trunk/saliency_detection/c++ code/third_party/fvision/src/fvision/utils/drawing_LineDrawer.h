#pragma once

#include <cv.h>

namespace fvision {

class LineDrawer {
public:
	LineDrawer(CvScalar color = CV_RGB(0, 255, 0), int thickness = 1, int line_type = 8) {
		this->color = color;
		this->thickness = thickness;
		this->line_type = line_type;
	}

public:
	void draw(IplImage* src, const CvPoint& pt1, const CvPoint& pt2) const;
	void draw(IplImage* src, const CvPoint2D32f& pt1, const CvPoint2D32f& pt2) const;


public:
	CvScalar color;
	int thickness;
	int line_type;
};

}
