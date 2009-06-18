#include "PointCalculatorImpl.h"

#include <fvision/algorithm/dlt.h>
#include <fvision/utils/point_mat_conversion_template.h>

namespace fvision {

class PointConstraints : public ConstraintsConstructor<CameraPointSample> {

private:
	CvMat* A;

public:
	PointConstraints() {
		A = cvCreateMat(2, 4, CV_64FC1);
	}
	~PointConstraints() {
		cvReleaseMat(&A);
	}

public:
	const CvMat* construct(const CameraPointSample& sample) const {

		const CvMat* P = sample.first.getMat();
		double x = sample.second.x;
		double y = sample.second.y;

		cvmSet(A, 0, 0, x * cvmGet(P, 2, 0) - cvmGet(P, 0, 0));
		cvmSet(A, 0, 1, x * cvmGet(P, 2, 1) - cvmGet(P, 0, 1));
		cvmSet(A, 0, 2, x * cvmGet(P, 2, 2) - cvmGet(P, 0, 2));
		cvmSet(A, 0, 3, x * cvmGet(P, 2, 3) - cvmGet(P, 0, 3));

		cvmSet(A, 1, 0, y * cvmGet(P, 2, 0) - cvmGet(P, 1, 0));
		cvmSet(A, 1, 1, y * cvmGet(P, 2, 1) - cvmGet(P, 1, 1));
		cvmSet(A, 1, 2, y * cvmGet(P, 2, 2) - cvmGet(P, 1, 2));
		cvmSet(A, 1, 3, y * cvmGet(P, 2, 3) - cvmGet(P, 1, 3));

		return A;
	}

};

CvPoint3D64f DltPointCalculator::compute(const CameraPointSamples& samples) {
	PointConstraints c;
	GenericDlt<CameraPointSample> dlt;
	CvMat* mp = dlt.compute(samples, c);
	CvPoint3D64f p;
	hmatToPoint3D(mp, p);
	cvReleaseMat(&mp);
	return p;
}

}