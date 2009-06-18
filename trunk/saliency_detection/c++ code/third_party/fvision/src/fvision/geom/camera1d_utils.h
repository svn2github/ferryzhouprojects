#pragma once

#include <vector>
#include <cv.h>

namespace fvision {

//nx = (x - principalPoint) / focalLength
std::vector<double> normalizePointsWithCamera1DInternalMatrix (
	double focalLength, 
	double principalPoint, 
	const std::vector<double>& xs 
);

std::vector<double> project(const CvMat* M, const std::vector<CvPoint2D32f>& points);

//double project(const CvMat* M, const CvPoint2D32f& point);
}