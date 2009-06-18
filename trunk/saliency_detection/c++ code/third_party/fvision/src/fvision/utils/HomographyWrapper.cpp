#include "HomographyWrapper.h"

#include "cvmat_utils.h"

#include <cassert>

namespace fvision {

HomographyWrapper::HomographyWrapper(const CvMat* M) {
	assert(M->rows == M->cols);
	this->M = cvCloneMat(M);
	this->invM = inv(M);
	this->H = cvCreateMat(M->rows, M->cols, M->type);
	this->buf = cvCreateMat(M->rows, M->cols, M->type);
}

HomographyWrapper::~HomographyWrapper(void) {
	clearMats(&M, &invM, &H, &buf);
}

const CvMat* HomographyWrapper::wrap(const CvMat* T) {
	cvMatMul(T, invM, buf);
	cvMatMul(M, buf, H);
	return H;
}

}