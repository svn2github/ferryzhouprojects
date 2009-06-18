#include "camera_utils.h"

#include <fvision/utils/point_mat_conversion_template.h>
#include <fvision/utils/cvmat_utils.h>
#include <fvision/utils/geom.h>

namespace fvision {

CvPoint2D32f getPrincipalPointFromImageSize(const CvSize& imSize) {
	return cvPoint2D32f(imSize.width / 2, imSize.height / 2);
}

double getFocalLengthFromVerticalFovAndImageHeight(double vFov, double height) {
	return height / 2 / tan(vFov / 2);
}

double getFocalLengthFromHorizontalFovAndImageWidth(double hFov, double width) {
	return width / 2 / tan(hFov / 2);
}

double getVerticalFovFromFocalLengthAndImageHeight(double focalLength, double height) {
	return atan2(height, 2 * focalLength) * 2;
}


CvMat* getKFromFocalLengthAndPrinciplePoint(double focalLength, const CvPoint2D32f& pp) {
	CvMat* K = getIdentity();
	cvmSet(K, 0, 0, focalLength);
	cvmSet(K, 1, 1, -focalLength);
	cvmSet(K, 0, 2, pp.x);
	cvmSet(K, 1, 2, pp.y);
	return K;
}

CvMat* getKFromFocalLengthAndImageSize(double focalLength, const CvSize& imSize) {
	CvPoint2D32f pp = getPrincipalPointFromImageSize(imSize);
	return getKFromFocalLengthAndPrinciplePoint(focalLength, pp);
}

CvMat* getKFromVerticalFovAndImageSize(double vFov, const CvSize& imSize) {
	double focalLength = getFocalLengthFromVerticalFovAndImageHeight(vFov, imSize.height);
	return getKFromFocalLengthAndImageSize(focalLength, imSize);
}

CvMat* getKFromHorizontalFovAndImageSize(double hFov, const CvSize& imSize) {
	double focalLength = getFocalLengthFromHorizontalFovAndImageWidth(hFov, imSize.width);
	return getKFromFocalLengthAndImageSize(focalLength, imSize);
}

CvMat* getKFromImageWidthHeightAndWidthAlpha(int width, int height, double alpha) {
	return getKFromFocalLengthAndImageSize(alpha * width, cvSize(width, height));
}


//new focal length = scale * old focal length
CvMat* getScaledK(CvMat* K, double scale) {
	CvMat* sK = cvCloneMat(K);
	cvmSet(sK, 0, 0, scale * cvmGet(K, 0, 0));
	cvmSet(sK, 1, 1, scale * cvmGet(K, 1, 1));
	return sK;
}


// z = l / ||l||;  x = (u x l) / ||u x l||, y = z x x
// R = [x, y, z]'
CvMat* getRInLeftHandSystem(const CvPoint3D64f& location, const CvPoint3D64f& lookat, const CvPoint3D64f& up) {
	CvPoint3D64f viewDirection = lookat - location;
	CvMat* z = matFromPoint3D(viewDirection);
	CvMat* u = matFromPoint3D(up);
	normalizeVector(z);
	CvMat* x = crossProduct(u, z);
	normalizeVector(x);
	CvMat* y = crossProduct(z, x);

	CvMat* Rt = cvCreateMat(3, 3, CV_64FC1);
	setCol(Rt, x, 0);
	setCol(Rt, y, 1);
	setCol(Rt, z, 2);
	CvMat* R = transpose(Rt);

	cvReleaseMat(&x);
	cvReleaseMat(&y);
	cvReleaseMat(&z);
	cvReleaseMat(&u);
	cvReleaseMat(&Rt);

	return R;
}

CvMat* getStandardNP1() {
	CvMat* NP = cvCreateMat(3, 4, CV_64FC1);
	cvZero(NP);
	CvMat* I = getIdentity();
	setCols(NP, I, 0, 3);
	cvReleaseMat(&I);
	return NP;
}

//[R|t]
CvMat* getNP(const CvMat* R, const CvMat* t) {
	CvMat* NP = cvCreateMat(3, 4, CV_64FC1);
	setCols(NP, R, 0, 3);
	setCol(NP, t, 3);
	return NP;
}

CvMat* getP(const CvMat* K, const CvMat* R, const CvMat* t) {

	CvMat* NP = getNP(R, t);

	CvMat* P = matMul(K, NP);

	cvReleaseMat(&NP);

	return P;
}

CvMat* getP(const CvMat* K, const CvMat* R, const CvPoint3D64f& C) {
	CvMat* mc = matFromPoint3D(C);
	CvMat* t = matMul(R, mc);
	cvScale(t, t, -1);
	CvMat* P = getP(K, R, t);
	cvReleaseMat(&mc);
	cvReleaseMat(&t);
	return P;
}

}