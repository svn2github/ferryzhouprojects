#include "camera1d_utils.h"

#include <fvision/utils/point_mat_conversion_template.h>

using namespace std;

namespace fvision {

std::vector<double> normalizePointsWithCamera1DInternalMatrix(
	double focalLength, 
	double principalPoint, 
	const std::vector<double>& xs 
) {
	vector<double> nxs(xs.size());
	for (size_t i = 0; i < xs.size(); i++) {
		nxs[i] = (xs[i] - principalPoint) / focalLength;
	}
	return nxs;
}
/*
double project(const CvMat* M, const CvPoint2D32f& point) {
	CvMat* mp = cvCreateMat(3, 1, CV_64FC1);
	hmatFromPoint2D(point, mp);
	CvMat* n = matMul(M, mp);

}
*/
std::vector<double> project(const CvMat* M, const std::vector<CvPoint2D32f>& points) {
	vector<double> nps(points.size());
	CvMat* mp = cvCreateMat(3, 1, CV_64FC1);
	CvMat* nmp = cvCreateMat(2, 1, CV_64FC1);
	for (size_t i = 0; i < points.size(); i++) {
		hmatFromPoint2D(points[i], mp);
		cvMatMul(M, mp, nmp);
		nps[i] = cvmGet(nmp, 0, 0) / cvmGet(nmp, 1, 0);
	}
	cvReleaseMat(&mp);
	cvReleaseMat(&nmp);
	return nps;
}
}