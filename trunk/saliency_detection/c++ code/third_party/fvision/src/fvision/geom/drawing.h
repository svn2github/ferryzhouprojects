#pragma once

#include "Line.h"

#include <fvision/utils.h>

#include <vector>

namespace fvision {
	void drawLine(IplImage* im, const Line& line, const LineDrawer& ld);
	void drawLines(IplImage* im, const std::vector<Line>& lines, const LineDrawer& ld);
	void drawLinesWithId(IplImage* im, const std::vector<Line>& lines, const std::vector<int>& ids, const LineDrawer& ld, const TextDrawer& td);
	void drawLinesWithId(IplImage* im, const std::vector<Line>& lines, const LineDrawer& ld, const TextDrawer& td);
}
