#pragma once

#include "Line.h"

#include <vector>

namespace fvision {

/**
 * input a set of image lines with ids
 * output vertical lines and horizontal lines with ids
 */
class VhLineFilter {
public:
	VhLineFilter(double vAngleThreshold = CV_PI / 6);
	~VhLineFilter(void);

public:
	void filter(const std::vector<Line>& lines, const std::vector<int>& ids);

private:
	void clearAll();

public:
	std::vector<Line> verticalLines;
	std::vector<int> verticalIds;
	std::vector<Line> horizontalLines;
	std::vector<int> horizontalIds;

private:
	double vAngleThreshold;
};

}