
#include "CornerPointMatchTwoViewPointTracker.h"

#include <cassert>

using namespace std;

namespace fvision {

void CornerPointMatchTwoViewPointTracker::compute(const IplImage* im1, 
												  const IplImage* im2, 
												  vector<CvPoint>& x1s, 
												  vector<CvPoint>& x2s, 
												  const vector<CvPoint>* pcorners1) 
{
	CvSize size1 = cvGetSize(im1);
	CvSize size2 = cvGetSize(im2);
	assert(size1.height == size2.height && size1.width == size2.width && im1->nChannels == im2->nChannels);

	corners1.clear();
	corners2.clear();

	if (pcorners1 == NULL) {
		pcd->compute(im1, corners1);
	} else {
		corners1.insert(corners1.begin(), pcorners1->begin(), pcorners1->end());
	}
	pcd->compute(im2, corners2);

	ppm->match(im1, im2, corners1, corners2, x1s, x2s);

}


void CornerPointMatchTwoViewPointTracker::compute(const IplImage* im1, 
												  const IplImage* im2, 
												  std::vector<CvPoint2D32f>& x1s, 
												  std::vector<CvPoint2D32f>& x2s) 
{
	vector<CvPoint> tx1s, tx2s;
	compute(im1, im2, tx1s, tx2s);
	size_t size = corners1.size();
	x1s.resize(size);
	x2s.resize(size);
	for (size_t i = 0; i < size; i++) {
		x1s[i] = cvPointTo32f(corners1[i]);
		x2s[i] = cvPointTo32f(corners2[i]);
	}
}

}