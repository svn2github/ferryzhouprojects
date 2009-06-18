#include "CameraInternal.h"

#include "PointTransformerFactory.h"

#include <fvision/utils/cvmat_utils.h>

#include <cassert>

namespace fvision {

CameraInternal::CameraInternal() {
	this->K = NULL;
	this->k = NULL;
	CvMat* I = getIdentity();
	initWith(I, k, true);	
	cvReleaseMat(&I);
}

CameraInternal::CameraInternal(const CvMat* K, const CvMat* k) {
	this->K = NULL;
	this->k = NULL;
	initWith(K, k, true);
}

CameraInternal::~CameraInternal(void) {
	cvReleaseMat(&K);
	if (k != NULL) cvReleaseMat(&k);
}

void CameraInternal::initWith(const CvMat* K, const CvMat* k, bool toNormalize) {

	assert(K != NULL);

	this->K = cvCloneMat(K);
	this->toNormalize = toNormalize;

	PointTransformerFactory factory;

	this->pknorm   = factory.createKNormalizer(K);
	this->pkdenorm = factory.createKDeNormalizer(K);
	
	if (k != NULL) {
		this->k = cvCloneMat(k);
		pundist = new NormalizedPointUndistorter(k);
	} else {
		this->k = NULL;
		pundist = NULL;
	}
}

CameraInternal::CameraInternal(const CameraInternal& rhs) {
	initWith(rhs.K, rhs.k, rhs.toNormalize);
}

CameraInternal& CameraInternal::operator= (const CameraInternal& rhs) {
	if (this == &rhs) return *this;
	if (K != NULL) cvReleaseMat(&K);
	if (k != NULL) cvReleaseMat(&k);
	initWith(rhs.K, rhs.k, rhs.toNormalize);
	return *this;
}

CvPoint2D32f CameraInternal::operator()(const CvPoint2D32f& p) const {
	if (toNormalize) return normalize(p);
	else return deNormalize(p);
}

CvPoint2D32f CameraInternal::normalize(const CvPoint2D32f& p) const {
	CvPoint2D32f np = (*pknorm)(p);
	if (pundist != NULL) np = (*pundist)(np);
	return np;
}

CvPoint2D32f CameraInternal::deNormalize(const CvPoint2D32f& np) const {
	CvPoint2D32f p = (*pkdenorm)(np);
	if (k == NULL) return p;
	else {
		CvPoint2D32f dp = distort(np, k);
		return (*pkdenorm)(dp);
	}
}

ImagePointUndistorter CameraInternal::getImagePointUndistorter() const {
	CvPoint2D32f center = cvPoint2D32f(cvmGet(K, 0, 2), cvmGet(K, 1, 2));
	return ImagePointUndistorter(center, k);
}

const PointTransformer* CameraInternal::getKDeNormalizer() const {
	return this->pkdenorm;
}

}