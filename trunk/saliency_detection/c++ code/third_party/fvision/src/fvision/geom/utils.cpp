#include "utils.h"

using namespace std;

namespace fvision {

std::vector<CvMat*> getLineMats(std::vector<Line>& lines) {
	vector<CvMat*> mlines;
	for (size_t i = 0; i < lines.size(); i++) {
		mlines.push_back(lines[i].getMat());
	}
	return mlines;
}

}