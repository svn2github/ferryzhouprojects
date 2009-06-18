#pragma once

#include <cv.h>
#include <vector>

namespace fvision {

void computeExtrinsicCamera1DParameters (
	double focalLength, 
	double principalPoint, 
	const std::vector<CvPoint2D32f>& Xs, 
	const std::vector<double>& xs, 
	double& camAngle,
	CvPoint2D32f& camPosition
);

//normalized coordinates, i.e. f = 1, px = 0;
//nx = R * [X - C] = R[I|-C]X = MX
void computeExtrinsicCamera1DParametersWithNormalizedCoordinates (
	const std::vector<CvPoint2D32f>& Xs, 
	const std::vector<double>& nxs,
	double& camAngle,
	CvPoint2D32f& camPosition);

//Get Camera Matrix from 3d -> 2d correspondences
//x ~= M * X
//at least 6 correspondences
//DLT method
CvMat* computeCamera1DMatrixFrom2DTo1DMapping(const std::vector<CvPoint2D32f>& Xs, const std::vector<double>& xs);

}