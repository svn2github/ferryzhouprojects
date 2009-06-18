#include "drawing_PointPairsDrawer.h"

using namespace std;

namespace fvision {

PointPairsDrawer::PointPairsDrawer(void)
{
}

PointPairsDrawer::~PointPairsDrawer(void)
{
}

void PointPairsDrawer::draw(IplImage* im, const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s) {
	assert (x1s.size() == x2s.size());
	for (size_t i = 0; i < x1s.size(); i++) {
		draw(im, x1s[i], x2s[i]);
	}
}

void PointPairsDrawer::draw(IplImage* im, const CvPoint2D32f& x1, const CvPoint2D32f& x2) {
	pointDrawer1.draw(im, x1);
	pointDrawer2.draw(im, x2);
	lineDrawer.draw(im, x1, x2);
}
}