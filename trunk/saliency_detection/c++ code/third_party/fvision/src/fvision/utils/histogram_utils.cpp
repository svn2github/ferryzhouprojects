#include "histogram_utils.h"

#include <cassert>

namespace fvision {

double computeHistogramCorrelation(const CvMat* hist1, const CvMat* hist2) {
	assert(hist1->rows == hist2->rows && hist1->cols == 1 && hist2->cols == 1);
	int bins = hist1->rows;
	double sum = 0;
	int aij = 0;
	double asum = 0;
	for (int i = 0; i < bins; i++) {
		double i_diff = abs(cvmGet(hist1, i, 0) - cvmGet(hist2, i, 0));
		for (int j = 0; j < bins; j++) {
			aij = (i - j) * (i - j);
			double j_diff = abs(cvmGet(hist1, j, 0) - cvmGet(hist2, j, 0));
			sum += aij * i_diff * j_diff;
			asum += aij;
		}
	}
	return sqrt(sum / asum);
}
}