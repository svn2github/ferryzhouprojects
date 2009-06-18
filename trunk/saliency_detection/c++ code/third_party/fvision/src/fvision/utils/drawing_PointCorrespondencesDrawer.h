#pragma once

#include <cv.h>

#include "pointpair.h"

namespace fvision {

class PointDrawer;
class LineDrawer;

class PointCorrespondencesDrawer {
public:
	PointCorrespondencesDrawer(void);
	~PointCorrespondencesDrawer(void);

public:
	IplImage* create(const IplImage* leftImage, const IplImage* rightImage, const PointPairs& pps) const;
	IplImage* create(const IplImage* leftImage, const PointPairs& pps) const;

	IplImage* create(const IplImage* leftImage, const IplImage* rightImage, const std::vector<CvPoint2D32f>& x1s, const std::vector<CvPoint2D32f>& x2s) const;
	IplImage* create(const IplImage* leftImage, const std::vector<CvPoint2D32f>& x1s, const std::vector<CvPoint2D32f>& x2s) const;

	void draw(IplImage* leftImage, const PointPairs& pps) const;
	void draw(IplImage* leftImage, const std::vector<CvPoint2D32f>& x1s, const std::vector<CvPoint2D32f>& x2s) const;

	void setPointDrawer(const PointDrawer& pd);
	void setLineDrawer(const LineDrawer& ld);

private:
	PointDrawer* pointDrawer;
	LineDrawer* lineDrawer;
};

}