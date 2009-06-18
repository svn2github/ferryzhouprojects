#include "drawing_LineDrawer.h"


namespace fvision {


void LineDrawer::draw(IplImage* src, const CvPoint& pt1, const CvPoint& pt2) const {
	cvLine(src, pt1, pt2, color, thickness, line_type);
}

void LineDrawer::draw(IplImage* src, const CvPoint2D32f& pt1, const CvPoint2D32f& pt2) const {
	draw(src, cvPointFrom32f(pt1), cvPointFrom32f(pt2));
}

}