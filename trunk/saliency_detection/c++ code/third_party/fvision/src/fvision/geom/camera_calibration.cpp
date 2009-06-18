#include "camera_calibration.h"

#include "rotation.h"
#include "camera_utils.h"

#include <fvision/utils/point_mat_conversion_template.h>
#include <fvision/utils/cvmat_utils.h>
#include <fvision/utils/geom.h>

using namespace std;

namespace fvision {

double computeFocalLengthFromOrthogonalVanishingPoint(const CvPoint2D32f& v1, const CvPoint2D32f& v2, const CvPoint2D32f& pp) {
	double t = -((v1.x  - pp.x) * v2.x + (v1.y - pp.y) * v2.y - v1.x * pp.x - v1.y * pp.y + pp.y * pp.y + pp.x * pp.x);
	if (t < 0) return 0; //error!
	return sqrt(t);
}


CvMat* computeRFromNormalizedVerticalVanishingPoint(CvMat* nvp) {

	CvMat* invR = getXZRToYAxisInfinity(nvp);
	CvMat* R = inv(invR);

	cvReleaseMat(&invR);

	return R;
}

CvMat* computeRFromVerticalVanishingPoint(CvMat* verticalVanishingPoint, CvMat* K) {
	CvMat* invK = inv(K);
	CvMat* np = matMul(invK, verticalVanishingPoint);
	CvMat* R = computeRFromNormalizedVerticalVanishingPoint(np);

	cvReleaseMat(&invK);
	cvReleaseMat(&np);
	return R;
}

CvMat* computeRFromHorizontalInfiniteLine(CvMat* line, CvMat* K) {
	CvMat* Kt = transpose(K);
	CvMat* nl8 = matMul(Kt, line);
	CvMat* R = computeRFromNormalizedVerticalVanishingPoint(nl8);

	cvReleaseMat(&Kt);
	cvReleaseMat(&nl8);
	return R;
}

void computeKR(const CvPoint2D32f& verticalVanishingPoint, const CvPoint2D32f& horizontalVanishingPoint, const CvPoint2D32f& pp, CvMat** pK, CvMat** pR) {
	double focalLength = computeFocalLengthFromOrthogonalVanishingPoint(verticalVanishingPoint, horizontalVanishingPoint, pp);
	*pK = getKFromFocalLengthAndPrinciplePoint(focalLength, pp);
	*pR = computeRFromVerticalVanishingPoint(hmatFromPoint2D(verticalVanishingPoint), *pK);
}

//bug: what if vanishing points are at infinity?
void computeKR(const CvPoint2D32f pts[8], const CvPoint2D32f& pp, CvMat** pK, CvMat** pR) {
	vector<CvMat*> lines;
	for (int i = 0; i < 4; i++) {
		CvMat* line = getLine(pts[i], pts[i + 1]);
		lines.push_back(line);
	}
	CvMat* vvpm = crossProduct(lines[0], lines[1]);
	CvPoint2D32f vvp;
	hmatToPoint2D(vvpm, vvp);
	CvMat* hvpm = crossProduct(lines[2], lines[3]);
	CvPoint2D32f hvp;
	hmatToPoint2D(hvpm, hvp);
	computeKR(vvp, hvp, pp, pK, pR);

	clearMat(&vvpm);
	clearMat(&hvpm);
	clearMatVector(lines);
}

double computeYThetaFromNormalizedHorizontalLinePair(const CvMat* nhLine1, const CvMat* nhLine2) {
	double a1 = cvmGet(nhLine1, 0, 0);
	double c1 = cvmGet(nhLine1, 2, 0);
	double a2 = cvmGet(nhLine2, 0, 0);
	double c2 = cvmGet(nhLine2, 2, 0);

	return getTheta(c2 * a1 - a2 * c1, c1 * c2 + a1 * a2);
}

//A * p = 0
//Each correspondence add two constraints to P
CvMat* computePFrom3DTo2DMapping(const vector<CvPoint3D64f>& Xs, const vector<CvPoint2D32f>& xs) {
	int num = (int)xs.size();
	CvMat* A = cvCreateMat(num * 2, 12, CV_64FC1);
	cvZero(A);
	for (int i = 0; i < (int)xs.size(); i++) {
		double x3 = Xs[i].x;
		double y3 = Xs[i].y;
		double z3 = Xs[i].z;

		double x2 = xs[i].x;
		double y2 = xs[i].y;

		cvmSet(A, i * 2, 0, x3);
		cvmSet(A, i * 2 , 1, y3);
		cvmSet(A, i * 2, 2, z3);
		cvmSet(A, i * 2, 3, 1);

		cvmSet(A, i * 2, 8, -x3 * x2);
		cvmSet(A, i * 2, 9, -y3 * x2);
		cvmSet(A, i * 2, 10, -z3 * x2);
		cvmSet(A, i * 2, 11, -x2);

		cvmSet(A, i * 2 + 1, 4, x3);
		cvmSet(A, i * 2 + 1, 5, y3);
		cvmSet(A, i * 2 + 1, 6, z3);
		cvmSet(A, i * 2 + 1, 7, 1);

		cvmSet(A, i * 2 + 1, 8, -x3 * y2);
		cvmSet(A, i * 2 + 1, 9, -y3 * y2);
		cvmSet(A, i * 2 + 1, 10, -z3 * y2);
		cvmSet(A, i * 2 + 1, 11, -y2);
	}

	CvMat* p = null(A);

	CvMat* P = cvCreateMat(3, 4, CV_64FC1);

	for (int i = 0; i < 12; i++) {
		P->data.db[i] = p->data.db[i];
	}

	cvReleaseMat(&A);
	cvReleaseMat(&p);

	return P;
}

CvMat* computePFrom3DTo2DMapping(CvMat* K, CvMat* k, const vector<CvPoint3D64f>& Xs, const vector<CvPoint2D32f>& xs) {
	CvMat* R = cvCreateMat(3, 3, CV_64FC1);
	CvMat* t = cvCreateMat(3, 1, CV_64FC1);
	computeExtrinsicCameraParameters(K, k, Xs, xs, R, t);
	CvMat* P = getP(K, R, t);
	cvReleaseMat(&R);
	cvReleaseMat(&t);
	return P;
}

void computeExtrinsicCameraParameters(CvMat* K, CvMat* k, const vector<CvPoint3D64f>& Xs, const vector<CvPoint2D32f>& xs, CvMat* R, CvMat* t) {
	CvMat* object_points = matFromPoint3DVector(Xs);
	CvMat* image_points = matFromPoint2DVector(xs);
	CvMat* r = cvCreateMat(3, 1, CV_64FC1);
	cvFindExtrinsicCameraParams2(object_points, image_points, K, k, r, t);
	cvRodrigues2(r, R);
	cvReleaseMat(&object_points);
	cvReleaseMat(&image_points);
	cvReleaseMat(&r);
}

}