#ifndef FVISION_GENERIC_DLT_H_
#define FVISION_GENERIC_DLT_H_

#include <cv.h>
#include <vector>

#include <fvision/utils/cvmat_utils.h>

namespace fvision {

template <typename Sample>
class ConstraintsConstructor {
public:
	/** for each sample, construct one or more constraints (rows), which is represented by a matrix
	 * the returned object is released by the class itself
	 */
	virtual const CvMat* construct(const Sample& sample) const = 0;
};

/** Generic DLT algorithm
 * @see DltPointCalculator, DltFundamentalMatrixCalculator, DltHomographyCalculator, DltLineSegmentIntersectionCalculator
 */ 
template <typename Sample>
class GenericDlt {
public:
	typedef std::vector<Sample> Samples;
	/**
	 * stack all constratins together and solve the equations
	 * the user is responsible for deleting the returing object
	 */
	CvMat* compute(const Samples& samples, const ConstraintsConstructor<Sample>& constructor) {
		assert(samples.size() > 0);
		const CvMat* constraint = constructor.construct(samples[0]);
		int rows = constraint->rows;
		int cols = constraint->cols;
		CvMat* A = cvCreateMat((int)samples.size() * rows, cols, constraint->type);
		for (size_t i = 0; i < samples.size(); i++) {
			constraint = constructor.construct(samples[i]);
			fvision::setSubRect(A, constraint, cvRect(0, (int)i * rows, cols, rows));
		}
		CvMat* N = fvision::null(A);
		fvision::clearMat(&A);
		return N;
	}
};
}
#endif // FVISION_GENERIC_DLT_H_