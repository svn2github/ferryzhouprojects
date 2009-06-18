#include "Camera1D.h"

#include <fvision/utils/geom.h>
#include <fvision/utils/io.h>

#include <algorithm>

#include <cmath>

using namespace std;

namespace fvision {

Camera1DInternal::Camera1DInternal(double focalLength, double imageWidth) {
	this->focalLength  = focalLength;
	this->imageWidth   = imageWidth;
	this->px           = imageWidth / 2;
	this->fov          = 2 * atan2(px, focalLength);
}

fvision::Camera1DInternal Camera1DInternal::createWithFocalLengthAndImageWidth(double focalLength, double imageWidth) {
	return Camera1DInternal(focalLength, imageWidth);
}

fvision::Camera1DInternal Camera1DInternal::createWithFocalLengthAndPrincipalPoint(double focalLength, double principalPoint) {
	Camera1DInternal camInternal;
	camInternal.focalLength = focalLength;
	camInternal.imageWidth = focalLength;
	camInternal.px = principalPoint;
	camInternal.fov = 2 * atan(0.5);
	return camInternal;
}

CvPoint2D32f Camera1DExternal::transformPoint(const CvPoint2D32f& p) const {
	return rotatePoint(p - c, theta);
}

Camera1DExternal::Camera1DExternal() {
	theta = 0;
	c = cvPoint2D32f(0, 0);
}

Camera1D::Camera1D(double focalLength, double imageWidth, const CvPoint2D32f& c, double theta) : cameraInternal(focalLength, imageWidth), cameraExternal(c, theta) {
}

Camera1D::Camera1D(const Camera1DInternal& camInternal, const Camera1DExternal& camExternal) : cameraInternal(camInternal), cameraExternal(camExternal) {
}

Camera1D::Camera1D() {

}

Camera1D::~Camera1D(void)
{
}

std::vector<double> Camera1D::calculateImagePoints(const std::vector<CvPoint2D32f>& pts) const {
	vector<double> ixs;
	ixs.resize(pts.size());
	transform(pts.begin(), pts.end(), ixs.begin(), *this);
	return ixs;
}

std::vector<double> Camera1D::calculateImagePointsWithinImage(const std::vector<CvPoint2D32f>& pts, vector<int>& is) const {
	vector<double> ixs;
	for (int i = 0; i < (int)pts.size(); i++) {
		double ix = calculateImagePoint(pts[i]);
		if (isPoint1DInImage(ix)) {
			ixs.push_back(ix);
			is.push_back(i);
		}
	}
	return ixs;
}

fvision::Camera1D Camera1D::create(double focalLength, double principalPoint, const CvPoint2D32f& camPosition, double camAngle) {
	Camera1D cam1d;
	cam1d.cameraInternal = Camera1DInternal::createWithFocalLengthAndPrincipalPoint(focalLength, principalPoint);
	cam1d.cameraExternal = Camera1DExternal(camPosition, camAngle);
	return cam1d;
}

std::vector<double> Camera1D::project(const std::vector<CvPoint2D32f>& ps) const {
	vector<double> xs(ps.size());
	for (size_t i = 0; i < ps.size(); i++) xs[i] = project(ps[i]);
	return xs;
}

std::ostream& operator<<(std::ostream& os, const Camera1DInternal& cameraInternal) {
	os<<cameraInternal.getFocalLength()<<" "<<cameraInternal.getPrincipalPoint();
	return os;
}

std::ostream& operator<<(std::ostream& os, const Camera1DExternal& cameraExternal) {
	os<<cameraExternal.getAngle()<<" "<<cameraExternal.getPosition();
	return os;
}

std::ostream& operator<<(std::ostream& os, const Camera1D& camera) {
	os<<camera.cameraInternal<<" "<<camera.cameraExternal;
	return os;
}

std::istream& operator>>(std::istream& is, Camera1DExternal& cameraExternal) {
	double angle;
	CvPoint2D32f position;
	is>>angle>>position;
	cameraExternal.setAngle(angle);
	cameraExternal.setPosition(position);
	return is;
}
}


