#include "HomographyPointTransformer.h"

#include <fvision/utils/point_mat_conversion_template.h>

#include <cassert>

namespace fvision {

HomographyPointTransformer::HomographyPointTransformer(const CvMat* H) {
	assert(H != NULL);
	assert(H->rows == 3 && H->cols == 3);
	this->H = cvCloneMat(H);
	this->inputPointBuffer = cvCreateMat(3, 1, CV_64FC1);
	this->outputPointBuffer = cvCreateMat(3, 1, CV_64FC1);
}

HomographyPointTransformer::~HomographyPointTransformer(void) {
	cvReleaseMat(&H);
	cvReleaseMat(&inputPointBuffer);
	cvReleaseMat(&outputPointBuffer);
}

CvPoint2D32f HomographyPointTransformer::transform(const CvPoint2D32f& p) const {
	hmatFromPoint2D(p, inputPointBuffer);
	cvMatMul(H, inputPointBuffer, outputPointBuffer);
	CvPoint2D32f np;
	hmatToPoint2D(outputPointBuffer, np);
	return np;
}

}