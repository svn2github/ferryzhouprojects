#include ".\camera.h"

#include <fvision/utils/cvmat_utils.h>
#include <fvision/utils/io.h>
#include <fvision/utils/point_mat_conversion_template.h>

#include "camera_utils.h"
#include "transform_points_utils.h"
#include "distortion.h"
#include "singleviewgeom.h"

#include <algorithm>

using namespace std;

namespace fvision {

Camera::Camera(const CvMat* K, const CvMat* NP) {
	initWithKNP(K, NP);
}

Camera::Camera(const CvMat* K) {
	initWithK(K);
}

Camera::Camera(const CvMat* K, const CvMat* R, const CvMat* t) {
	initWithKRt(K, R, t);
}

Camera::Camera() {
	CvMat* K = getIdentity();
	initWithK(K);
	cvReleaseMat(&K);
}

Camera* Camera::createWithP(const CvMat* P) {
	Camera* camera = new Camera();
	cvCopy(P, camera->P);
	return camera;
}

Camera* Camera::createWithLookAt(const CvMat* K, const LookAtParams& params) {
	CvMat* R = getRInLeftHandSystem(params.location, params.lookat, params.up);
	CvMat* C = matFromPoint3D(params.location);
	CvMat* t = cvCreateMat(3, 1, CV_64FC1);
	cvMatMul(R, C, t);
	cvScale(t, t, -1);
	Camera* cam = new Camera(K, R, t);
	clearMats(&R, &C, &t);
	return cam;
}

Camera::Camera(const Camera& cam) {
	initWithCamera(cam);
}

Camera::~Camera() {
	clear();
}

void Camera::clear() {
	clearMats(&P, &NP, &K, &R, &C, &t);
}

Camera& Camera::operator=(const Camera& rhs) {
	if (this == &rhs) return *this;
	reset(rhs.getK(), rhs.getR(), rhs.gett());
	return *this;
}

void Camera::reset(const CvMat* K, const CvMat* R, const CvMat* t) {

	cvCopy(K, this->K);
	cvCopy(R, this->R);
	cvCopy(t, this->t);

	setCols(NP, R, 0, 3);
	setCol(NP, t, 3);

	P = matMul(K, NP);

	CvMat* invR = inv(R);
	cvMatMul(invR, t, C);
	cvScale(C, C, -1);
	cvReleaseMat(&invR);

	matToPoint3D(C, o);
}

void Camera::initMats() {
	K = getIdentity();
	R = getIdentity();
	t = cvCreateMat(3, 1, CV_64FC1);
	cvZero(t);

	NP = cvCreateMat(3, 4, CV_64FC1);
	setCols(NP, R, 0, 3);
	setCol(NP, t, 3);

	P = matMul(K, NP);

	C = cvCreateMat(3, 1, CV_64FC1);
	cvZero(C);

	o = cvPoint3D64f(0, 0, 0);
}

void Camera::initWithKRt(const CvMat* K, const CvMat* R, const CvMat* t) {

	initMats();

	reset(K, R, t);
}

void Camera::initWithK(const CvMat* K) {
	CvMat* I = getIdentity();
	CvMat* t = getZero(3, 1);
	initWithKRt(K, I, t);
	cvReleaseMat(&I);
	cvReleaseMat(&t);
}

void Camera::initWithKNP(const CvMat* K, const CvMat* NP) {

	CvMat* mR = cvCreateMat(3, 3, CV_64FC1);
	copyCols(NP, mR, 0, 3);
	
	CvMat* mt = cvCreateMat(3, 1, CV_64FC1);
	copyCol(NP, mt, 3);

	initWithKRt(K, mR, mt);

	cvReleaseMat(&mR);
	cvReleaseMat(&mt);
}

void Camera::initWithCamera(const Camera& cam) {
	this->P = cvCloneMat(cam.getP());
	this->NP = cvCloneMat(cam.getNP());
	this->K = cvCloneMat(cam.getK());
	this->R = cvCloneMat(cam.getR());
	this->C = cvCloneMat(cam.getC());
	this->t = cvCloneMat(cam.gett());
	this->o = cam.geto();
}

CvPoint2D32f Camera::project(CvPoint3D32f X) const {
	CvPoint2D32f x = projectWithP(P, X);
	return x;
}

vector<CvPoint2D32f> Camera::project(const vector<CvPoint3D32f>& Xs) const {
	vector<CvPoint2D32f> xs;
	for (size_t i = 0; i < Xs.size(); i++) xs.push_back(project(Xs[i]));
	return xs;
}

CvPoint2D32f Camera::project(CvPoint3D64f X) const {
	CvPoint2D32f x = projectWithP(P, X);
	return x;
}

vector<CvPoint2D32f> Camera::project(const vector<CvPoint3D64f>& Xs) const {
	vector<CvPoint2D32f> xs;
	for (size_t i = 0; i < Xs.size(); i++) xs.push_back(project(Xs[i]));
	return xs;
}

//P' = H * P
//K' = H * K
void Camera::trans(const CvMat* H) {
	CvMat* P2 = matMul(H, P); //memory leak!
	cvReleaseMat(&P);
	P = P2;
	CvMat* K2 = matMul(H, K);
	cvReleaseMat(&K);
	K = K2;
}

void Camera::transWithScale(double scale) {
	double mh[] = {scale, 0, 0, 0, scale, 0, 0, 0, 1};
	CvMat MH = cvMat(3, 3, CV_64FC1, mh);
	trans(&MH);
}

void Camera::transWithFlipY(double h) {
	double mh[] = {1, 0, 0, 0, -1, h - 1, 0, 0, 1};
	CvMat MH = cvMat(3, 3, CV_64FC1, mh);
	trans(&MH);
}

void Camera::setK(const CvMat* K) {
	cvCopy(K, this->K);
	cvMatMul(K, NP, P);
}

CvMat* Camera::getM(CvMat* buf) const {
	if (buf == NULL) buf = cvCreateMat(3, 3, CV_64FC1);
	cvGetCols(P, buf, 0, 3);
	return buf;
}

ostream& operator<<(ostream& os, const Camera& cam) {
	os<<cam.getK()<<endl;
	os<<cam.getR()<<endl;
	os<<cam.gett()<<endl;
	return os;
}

istream& operator>>(istream& is, Camera& cam) {
	CvMat* K = NULL;
	CvMat* R = NULL;
	CvMat* t = NULL;
	is>>&K>>&R>>&t;
	cam.reset(K, R, t);
	clearMat(&K);
	clearMat(&R);
	clearMat(&t);
	return is;
}

}