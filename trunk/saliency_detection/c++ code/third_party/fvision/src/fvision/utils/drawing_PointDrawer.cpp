#include "drawing_PointDrawer.h"

namespace fvision {

void PointDrawer::draw(IplImage *src, const CvPoint& pt) const {
	switch (pattern[0]) {
			case 'o': //circle
				cvCircle(src, pt, size, color, thickness, line_type);
				break;
			case '+': //plus sign
				cvLine(src, cvPoint(pt.x - size, pt.y), cvPoint(pt.x + size, pt.y), color, thickness, line_type);
				cvLine(src, cvPoint(pt.x, pt.y - size), cvPoint(pt.x, pt.y + size), color, thickness, line_type);
				break;
			case 's': //square
				cvRectangle(src, cvPoint(pt.x - size, pt.y - size), cvPoint(pt.x + size, pt.y + size), color, thickness, line_type);
				break;
			default:
				cvCircle(src, pt, size, color, thickness, line_type); break;
	}
}

void PointDrawer::draw(IplImage *src, const CvPoint2D32f& pt) const {
	draw(src, cvPointFrom32f(pt));
}

}