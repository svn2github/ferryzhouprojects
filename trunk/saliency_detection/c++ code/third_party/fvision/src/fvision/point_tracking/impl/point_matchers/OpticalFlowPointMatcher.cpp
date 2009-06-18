#include "OpticalFlowPointMatcher.h"

#include "../../matching_utils.h"

#include <algorithm>

using namespace std;

namespace fvision {

void OpticalFlowPointMatcher::match(const IplImage* im1, 
									const IplImage* im2, 
									const vector<CvPoint>& corners1, 
									const vector<CvPoint>& corners2, 
									vector<CvPoint>& x1s, 
									vector<CvPoint>& x2s) 
{

	for (size_t i = 0; i < corners1.size(); i++) {
		CvPoint p2 = search(im1, im2, corners1[i]);
		if (p2.x == -1) { //can't find
		} else {
			x1s.push_back(corners1[i]);
			x2s.push_back(p2);
		}
	}
}

CvPoint OpticalFlowPointMatcher::search(const IplImage* im1, 
										const IplImage* im2,
										const CvPoint& p1) 
{

	vector<CvPoint> cps;
	for (int i = -halfSearchWindowSize; i <= halfSearchWindowSize; i++) {
		for (int j = -halfSearchWindowSize; j <= halfSearchWindowSize; j++) {
			int x = p1.x + i;
			int y = p1.y + j;
			if (x < 0 || x >= im1->width) continue;
			if (y < 0 || y >= im1->height) continue;
			cps.push_back(cvPoint(x, y));
		}
	}

	vector<float> diffs;
	for (size_t i = 0; i < cps.size(); i++) {
		diffs.push_back(windowMatch(im1, im2, p1, cps[i], halfPatchWindowSize));
	}
	vector<float>::iterator iter = min_element(diffs.begin(), diffs.end());
	if (*iter > threshold) return cvPoint(-1, -1);
	return cps[iter - diffs.begin()];
}

}