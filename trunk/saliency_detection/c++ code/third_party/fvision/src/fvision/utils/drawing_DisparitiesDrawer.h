#pragma once

#include <cv.h>
#include <vector>

namespace fvision {

class PointDrawer;
class LineDrawer;

class DisparitiesDrawer {
public:
	DisparitiesDrawer();
	~DisparitiesDrawer();

public:
	template <typename T>
	void draw(IplImage* src, const std::vector<CvPoint2D32f>& points, const std::vector<T>& disparities) const {
		assert(points.size() == disparities.size());
		drawPoints(src, points, *pd);
		for (size_t i = 0; i < points.size(); i++) {
			CvPoint2D32f np = points[i];
			np.x += (float)disparities[i];
			ld->draw(src, points[i], np);
		}
	}

public:
	PointDrawer* pd;
	LineDrawer* ld;
};

}