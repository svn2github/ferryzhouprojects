#include "transform_lines_utils.h"

#include "PointBasedLineTransformer.h"
#include "PointTransformerFactory.h"

using namespace std;

namespace fvision {

std::vector<Line> transformLinesWithHomography(const CvMat* H, const std::vector<Line>& lines) {
	PointTransformerFactory factory;
	PointTransformer* pt = factory.createHomographyTransformer(H);
	LineTransformer* lt = new PointBasedLineTransformer(pt);
	vector<Line> nLines = lt->copyTransform(lines);
	delete lt;
	delete pt;
	return nLines;
}

}