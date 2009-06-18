#include "CvMatWrapper.h"

#include "cvmat_utils.h"
#include "io.h"

namespace fvision {

//clone mat
CvMatWrapper::CvMatWrapper(const CvMat* mat) {
	this->mat = cloneMat(mat);
}

CvMatWrapper::CvMatWrapper(const CvMatWrapper& mw) {
	this->mat = cloneMat(mw.mat);
}

CvMatWrapper::~CvMatWrapper() {
	if (mat != NULL) cvReleaseMat(&mat);
}

const CvMatWrapper& CvMatWrapper::operator=(const CvMatWrapper& rhs) {
	if (this == &rhs) return *this;
	if (mat == NULL) mat = cvCloneMat(rhs.mat);
	else {
		if (isMatHeaderEqual(mat, rhs.mat)) cvCopy(rhs.mat, mat);
		else {
			cvReleaseMat(&mat);
			mat = cvCloneMat(rhs.mat);
		}
	}
	return *this;
}

const CvMatWrapper& CvMatWrapper::operator=(const CvMat* rhs) {
	if (mat != NULL) cvReleaseMat(&mat);
	if (rhs == NULL) mat = NULL;
	mat = cvCloneMat(rhs);
	return *this;
}

std::ostream& operator<< (std::ostream& os, const CvMatWrapper& mw) {
	os<<mw.getMat();
	return os;
}

}