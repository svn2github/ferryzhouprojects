#pragma once

#include <cv.h>

namespace fvision {

/**
 * input two normalized histograms; N x 1 matrix with sum of elements equals to 1
 * sum(aij * |h1_i - h2_i| * |h1_j - h2_j|) / sum(aij)
 * aij = (i - j) * (i - j)
 */
double computeHistogramCorrelation(const CvMat* hist1, const CvMat* hist2);

}