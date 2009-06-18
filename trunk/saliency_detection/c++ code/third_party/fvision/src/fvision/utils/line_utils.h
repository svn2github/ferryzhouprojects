#pragma once

#include "CvPointIterator.h"

#include <vector>

namespace fvision {

class LinePointsIterator : public CvPointIterator {
public:
	LinePointsIterator(int x0, int y0, int x1, int y1);

public:
	bool hasNext() const;
	const CvPoint& next();

private:
	CvPoint p;
	std::vector<CvPoint> points;
	std::vector<CvPoint>::const_iterator iter;
};

class LineSidePointsIterator : public CvPointIterator {
public:
	LineSidePointsIterator(int x0, int y0, int x1, int y1, bool rightSide, int rows);

public:
	bool hasNext() const;
	const CvPoint& next();

private:
	CvPoint p;
	std::vector<CvPoint> points;
	std::vector<CvPoint>::const_iterator iter;
};

std::vector<CvPoint> getLinePoints(int x0, int y0, int x1, int y1);

/**
 *	if steep, (x, y) -> (x+i, y)
 *	else (x, y + i)
 */
std::vector<CvPoint> getLineSidePoints(int x0, int y0, int x1, int y1, bool rightSide, int rows);

}