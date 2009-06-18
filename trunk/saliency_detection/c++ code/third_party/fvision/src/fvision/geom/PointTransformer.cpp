#include "PointTransformer.h"

#include <algorithm>

using namespace std;

namespace fvision {

PointTransformer::PointTransformer(void)
{
}

PointTransformer::~PointTransformer(void)
{
}

std::vector<CvPoint2D32f> PointTransformer::copyTransform(const std::vector<CvPoint2D32f>& ps) const {
	vector<CvPoint2D32f> nps(ps.size());
	for (size_t t = 0; t < ps.size(); t++) {
		nps[t] = transform(ps[t]);
	}
	return nps;
}

void PointTransformer::modifyTransform(std::vector<CvPoint2D32f>& ps) const {
	for (size_t t = 0; t < ps.size(); t++) {
		ps[t] = transform(ps[t]);
	}
}

}