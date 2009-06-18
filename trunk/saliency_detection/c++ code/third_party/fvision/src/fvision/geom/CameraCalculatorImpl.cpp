#include "CameraCalculatorImpl.h"

#include "camera_calibration.h"

#include <fvision/geom/singleviewgeom.h>
#include <fvision/utils/cvmat_utils.h>
#include <fvision/utils/template_utils.h>

using namespace std;

namespace fvision {

CalibratedCameraCalculator::CalibratedCameraCalculator(const CvMat* K, const CvMat* k) {
	this->K = cloneMat(K);
	this->k = cloneMat(k);
}

CalibratedCameraCalculator::CalibratedCameraCalculator() {
	this->K = getIdentity();
	this->k = NULL;
}

CalibratedCameraCalculator::~CalibratedCameraCalculator() {
	clearMat(&K);
	clearMat(&k);
}

Camera CalibratedCameraCalculator::compute(const ProjectionSamples& samples) {
	vector<CvPoint3D64f> Xs;
	vector<CvPoint2D32f> xs;
	decomposePairs(samples, Xs, xs);
	CvMat* R = cvCreateMat(3, 3, CV_64FC1);
	CvMat* t = cvCreateMat(3, 1, CV_64FC1);
	computeExtrinsicCameraParameters(K, k, Xs, xs, R, t);
	Camera cam(K, R, t);
	cvReleaseMat(&R);
	cvReleaseMat(&t);
	return cam;
}



Camera UnCalibratedCameraCalculator::compute(const ProjectionSamples& samples) {
	vector<CvPoint3D64f> Xs;
	vector<CvPoint2D32f> xs;
	decomposePairs(samples, Xs, xs);
	CvMat* P = computePFrom3DTo2DMapping(Xs, xs);
	Camera cam(I, P);
	cvReleaseMat(&P);
	return cam;
}

UnCalibratedCameraCalculator::UnCalibratedCameraCalculator() {
	I = getIdentity();
}

UnCalibratedCameraCalculator::~UnCalibratedCameraCalculator() {
	cvReleaseMat(&I);
}

}