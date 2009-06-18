#ifndef FVISION_GEOM_TEMPLATE_H_
#define FVISION_GEOM_TEMPLATE_H_

#include <cv.h>

namespace fvision {

//buf is a 3 x 1 matrix
//line = p1 x p2
template <typename Point2D>
CvMat* getLine(const Point2D& p1, const Point2D& p2, CvMat* buf = NULL) {
	if (buf == NULL) buf = cvCreateMat(3, 1, CV_64FC1);
	cvmSet(buf, 0, 0, p1.y - p2.y);
	cvmSet(buf, 1, 0, p2.x - p1.x);
	cvmSet(buf, 2, 0, p1.x * p2.y - p1.y * p2.x);
	return buf;
}

//buf is a 3x3 matrix
//cov = p x p'
template <typename Point3D>
CvMat* getPoint3DCovMatrix(const Point3D& p, CvMat* buf = NULL) {
	if (buf == NULL) buf = cvCreateMat(3, 1, CV_64FC1);
	cvmSet(buf, 0, 0, p.x * p.x);
	cvmSet(buf, 0, 1, p.x * p.y);
	cvmSet(buf, 0, 2, p.x * p.z);
	cvmSet(buf, 1, 0, p.y * p.x);
	cvmSet(buf, 1, 1, p.y * p.y);
	cvmSet(buf, 1, 2, p.y * p.z);
	cvmSet(buf, 2, 0, p.z * p.x);
	cvmSet(buf, 2, 1, p.z * p.y);
	cvmSet(buf, 2, 2, p.z * p.z);
	return buf;
}

}
#endif // FVISION_GEOM_TEMPLATE_H_
