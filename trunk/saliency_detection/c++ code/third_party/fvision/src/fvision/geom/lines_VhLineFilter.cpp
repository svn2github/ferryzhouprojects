#include "lines_VhLineFilter.h"

#include <cassert>

namespace fvision {

VhLineFilter::VhLineFilter(double vAngleThreshold) {
	assert(vAngleThreshold > 0);
	this->vAngleThreshold = vAngleThreshold;
}

VhLineFilter::~VhLineFilter(void)
{
}

void VhLineFilter::filter(const std::vector<Line>& lines, const std::vector<int>& ids) {
	clearAll();
	for (size_t i = 0; i < ids.size(); i++) {
		int id = ids[i];
		Line line = lines[i];
		if (abs(line.getYAngle()) <= vAngleThreshold) { //vertical line
			verticalLines.push_back(line);
			verticalIds.push_back(id);
		} else {
			horizontalLines.push_back(line);
			horizontalIds.push_back(id);
		}
	}
}

void VhLineFilter::clearAll() {
	verticalLines.clear();
	verticalIds.clear();
	horizontalLines.clear();
	horizontalIds.clear();	
}
}