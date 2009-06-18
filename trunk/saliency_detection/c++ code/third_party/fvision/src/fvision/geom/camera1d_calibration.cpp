#include "camera1d_calibration.h"

#include "camera1d_utils.h"
#include "Camera1DCalculator.h"
#include <fvision/algorithm/dlt.h>

using namespace std;

namespace fvision {

void computeExtrinsicCamera1DParameters(
	double focalLength, 
	double principalPoint, 
	const std::vector<CvPoint2D32f>& Xs, 
	const std::vector<double>& xs, 
	double& camAngle,
	CvPoint2D32f& camPosition
) {
	vector<double> nxs = normalizePointsWithCamera1DInternalMatrix(focalLength, principalPoint, xs);
	computeExtrinsicCamera1DParametersWithNormalizedCoordinates(Xs, nxs, camAngle, camPosition);
}

//first reconstruct M using DLT
//then get R and C from M
//  T = M[:, 1:2]
//  USV' = T
//  R = UV'
//  s = (S(1, 1) + S(1, 2)) / 2
//  t = M[:, 3]
//  C = -inv(R) * t / s
//finally do non-linear optimization
void computeExtrinsicCamera1DParametersWithNormalizedCoordinates (
	const std::vector<CvPoint2D32f>& Xs, 
	const std::vector<double>& nxs,
	double& camAngle,
	CvPoint2D32f& camPosition
) {
	CvMat* M = computeCamera1DMatrixFrom2DTo1DMapping(Xs, nxs);

	CvMat* T = getCols(M, 0, 2);
	CvMat* U = cvCreateMat(2, 2, CV_64FC1);
	CvMat* W = cvCreateMat(2, 2, CV_64FC1);
	CvMat* Vt = cvCreateMat(2, 2, CV_64FC1);
	cvSVD(T, W, U, Vt, CV_SVD_V_T);

	CvMat* R = matMul(U, Vt);

	double s = (cvmGet(W, 0, 0) + cvmGet(W, 1, 1)) / 2;
	CvMat* t = getCol(M, 2);
	CvMat* invR = inv(R);
	CvMat* C = matMul(invR, t);
	cvScale(C, C, -1/s);

	camAngle = normalizeAngleToHalfPi(atan2(cvmGet(R, 1, 0), cvmGet(R, 0, 0)));
	camPosition.x = (float)cvmGet(C, 0, 0);
	camPosition.y = (float)cvmGet(C, 1, 0);
	clearMats(&M, &T, &U, &W, &Vt, &R, &t, &invR, &C);
}

//x ~= M * X
//[u] = [a, b, c]   [x]
//[v]   [d, e, f] * [y]
//                  [z]
//[xv, yv, zv, -xu, -yu, -zu] * [a, b, c, d, e, f]' = 0
//since here z = 1, v = 1, we have
//[x, y, 1, -xu, -yu, -u] * [a, b, c, d, e, f]' = 0
CvMat* computeCamera1DMatrixFrom2DTo1DMapping(const std::vector<CvPoint2D32f>& Xs, const std::vector<double>& xs) {
	Projection1DSamples samples;
	composePairs(samples, Xs, xs);
	class Camera1DConstraints : public ConstraintsConstructor<Projection1DSample> {
	public:
		CvMat* construct(const Projection1DSample& sample) const {
			double x = sample.first.x;
			double y = sample.first.y;
			double u = sample.second;
			CvMat* A = cvCreateMat(1, 6, CV_64FC1);
			cvmSet(A, 0, 0, x);
			cvmSet(A, 0, 1, y);
			cvmSet(A, 0, 2, 1);
			cvmSet(A, 0, 3, -x * u);
			cvmSet(A, 0, 4, -y * u);
			cvmSet(A, 0, 5, -u);
			return A;
		}
	};

	GenericDlt<Projection1DSample> dlt;
	CvMat* m = dlt.compute(samples, Camera1DConstraints());
	CvMat* M = cvCreateMat(2, 3, CV_64FC1);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			cvmSet(M, i, j, cvmGet(m, i * 3 + j, 0));
		}
	}
	cvReleaseMat(&m);
	return M;
}

}