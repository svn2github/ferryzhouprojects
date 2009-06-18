#pragma once

#include <cv.h>

namespace fvision {

//A = transpose(R) * R
//A: positive definite matrix
//if isUpper = true, R is an upper triangle matrix, otherwise a lower triangle
//output R
bool choleskyDecompose(CvMat* A, CvMat* R, bool isUpper);

}