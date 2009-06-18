#ifndef FVISION_POINT_PAIRS_DRAWER_H_
#define FVISION_POINT_PAIRS_DRAWER_H_

#include "drawing_PointDrawer.h"
#include "drawing_LineDrawer.h"

#include <vector>

namespace fvision {

class PointPairsDrawer {
public:
	PointPairsDrawer(void);
	~PointPairsDrawer(void);

public:
	PointDrawer pointDrawer1;
	PointDrawer pointDrawer2;
	LineDrawer  lineDrawer;

public:
	void draw(IplImage* im, const std::vector<CvPoint2D32f>& x1s, const std::vector<CvPoint2D32f>& x2s);
	void draw(IplImage* im, const CvPoint2D32f& x1, const CvPoint2D32f& x2);

};

}
#endif // FVISION_POINT_PAIRS_DRAWER_H_