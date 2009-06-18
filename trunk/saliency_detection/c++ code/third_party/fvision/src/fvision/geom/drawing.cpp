#include "drawing.h"

#include <cassert>

namespace fvision {

void drawLine(IplImage* im, const Line& line, const LineDrawer& ld) {
	ld.draw(im, line.getPoint1(), line.getPoint2());
}

void drawLines(IplImage* im, const std::vector<Line>& lines, const LineDrawer& ld) {
	for (size_t i = 0; i < lines.size(); i++) {
		drawLine(im, lines[i], ld);
	}
}

void drawLinesWithId(IplImage* im, const std::vector<Line>& lines, const std::vector<int>& ids, const LineDrawer& ld, const TextDrawer& td) {
	assert(lines.size() == ids.size());
	char numberText[20];
	for (size_t i = 0; i < lines.size(); i++) {
		drawLine(im, lines[i], ld);
		CvPoint2D32f midPoint = (lines[i].getPoint1() + lines[i].getPoint2()) / 2;
		CvPoint position = cvPoint((int)(midPoint.x) + 5, (int)midPoint.y - 5);
		sprintf_s(numberText, 20, "%d", ids[i]);
		td.draw(im, position, numberText);
	}
}

void drawLinesWithId(IplImage* im, const std::vector<Line>& lines, const LineDrawer& ld, const TextDrawer& td) {
	vector<int> ids;
	for (size_t i = 0; i < lines.size(); i++) {
		ids.push_back(i);
	}
	drawLinesWithId(im, lines, ids, ld, td);
}
}