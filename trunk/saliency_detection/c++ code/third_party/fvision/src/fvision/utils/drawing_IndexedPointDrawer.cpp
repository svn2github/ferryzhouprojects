#include "drawing_IndexedPointDrawer.h"

#include "geom.h"

namespace fvision {

IndexedPointDrawer::IndexedPointDrawer(void) {
	pointDrawer = PointDrawer("+");
	offset.x = 5;
	offset.y = 5;
}

IndexedPointDrawer::IndexedPointDrawer(const PointDrawer& pointDrawer, const TextDrawer& textDrawer, const CvPoint& offset) {
	this->pointDrawer = pointDrawer;
	this->textDrawer = textDrawer;
	this->offset = offset;
}

IndexedPointDrawer::~IndexedPointDrawer(void) {
}

void IndexedPointDrawer::drawPointAndId(IplImage* im, const CvPoint2D32f& pt, int id) {
	pointDrawer.draw(im, pt);
	sprintf_s(numberText, 20, "%d", id);
	textDrawer.draw(im, cvPointFrom32f(pt) + offset, numberText);
}

void IndexedPointDrawer::drawPointsAndIds(IplImage* im, const std::vector<CvPoint2D32f>& pts, const std::vector<int>& ids) {
	assert(pts.size() == ids.size());
	for (size_t i = 0; i < pts.size(); i++) {
		drawPointAndId(im, pts[i], ids[i]);
	}
}
}