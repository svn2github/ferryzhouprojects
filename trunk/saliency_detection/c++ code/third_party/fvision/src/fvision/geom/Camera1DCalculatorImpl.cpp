#include "Camera1DCalculatorImpl.h"

#include "camera1d_calibration.h"

#include <fvision/geom/singleviewgeom.h>
#include <fvision/utils/cvmat_utils.h>
#include <fvision/utils/template_utils.h>

using namespace std;

namespace fvision {

CalibratedCamera1DCalculator::CalibratedCamera1DCalculator(double focalLength, double principalPoint) {
	this->focalLength = focalLength;
	this->principalPoint = principalPoint;
}

CalibratedCamera1DCalculator::~CalibratedCamera1DCalculator() {
}

Camera1D CalibratedCamera1DCalculator::compute(const Projection1DSamples& samples) {
	vector<CvPoint2D32f> Xs;
	vector<double> xs;
	decomposePairs(samples, Xs, xs);
	double camAngle = 0;
	CvPoint2D32f camPosition;
	computeExtrinsicCamera1DParameters(focalLength, principalPoint, Xs, xs, camAngle, camPosition);
	Camera1D cam = Camera1D::create(focalLength, principalPoint, camPosition, camAngle);
	return cam;
}


//not implemented yet
Camera1D UnCalibratedCamera1DCalculator::compute(const Projection1DSamples& samples) {
	vector<CvPoint3D64f> Xs;
	vector<CvPoint2D32f> xs;
	//decomposePairs(samples, Xs, xs);
	//CvMat* P = computePFrom3DTo2DMapping(Xs, xs);
	Camera1D cam;
	return cam;
}

UnCalibratedCamera1DCalculator::UnCalibratedCamera1DCalculator() {
	I = getIdentity();
}

UnCalibratedCamera1DCalculator::~UnCalibratedCamera1DCalculator() {
	cvReleaseMat(&I);
}

}