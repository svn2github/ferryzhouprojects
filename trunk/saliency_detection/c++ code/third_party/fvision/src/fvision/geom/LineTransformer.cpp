#include "LineTransformer.h"

using namespace std;

namespace fvision {

LineTransformer::LineTransformer(void)
{
}

LineTransformer::~LineTransformer(void)
{
}

std::vector<Line> LineTransformer::copyTransform(const std::vector<Line>& lines) const {
	vector<Line> newLines(lines.size());
	for (size_t t = 0; t < lines.size(); t++) {
		newLines[t] = transform(lines[t]);
	}
	return newLines;
}

void LineTransformer::modifyTransform(std::vector<Line>& lines) const {
	for (size_t t = 0; t < lines.size(); t++) {
		lines[t] = transform(lines[t]);
	}
}
}