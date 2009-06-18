#pragma once

#include <cv.h>
#include <vector>

namespace fvision {

//pp: principle point
//v1, v2, orthogonal vanishing point pair
double computeFocalLengthFromOrthogonalVanishingPoint(const CvPoint2D32f& v1, const CvPoint2D32f& v2, const CvPoint2D32f& pp);


//R
CvMat* computeRFromNormalizedVerticalVanishingPoint(CvMat* nvp);

CvMat* computeRFromVerticalVanishingPoint(CvMat* verticalVanishingPoint, CvMat* K);

CvMat* computeRFromHorizontalInfiniteLine(CvMat* line, CvMat* K);

void computeKR(const CvPoint2D32f& verticalVanishingPoint, const CvPoint2D32f& horizontalVanishingPoint, const CvPoint2D32f& pp, CvMat** pK, CvMat** pR);

//two points determine a line, two lines determin a point
//first four points determine the vertical vanishing point
//last four points determine the horizontal vanishing point
void computeKR(const CvPoint2D32f pts[8], const CvPoint2D32f& pp, CvMat** pK, CvMat** pR);

double computeYThetaFromNormalizedHorizontalLinePair(const CvMat* nhLine1, const CvMat* nhLine2);

//Get Camera Matrix from 3d -> 2d correspondences
//x ~= P * X
//at least 6 correspondences
//DLT method
CvMat* computePFrom3DTo2DMapping(const std::vector<CvPoint3D64f>& Xs, const std::vector<CvPoint2D32f>& xs);

CvMat* computePFrom3DTo2DMapping(CvMat* K, CvMat* k, const std::vector<CvPoint3D64f>& Xs, const std::vector<CvPoint2D32f>& xs);

//R: 3x3
//t: 3x1
void computeExtrinsicCameraParameters(CvMat* K, CvMat* k, const std::vector<CvPoint3D64f>& Xs, const std::vector<CvPoint2D32f>& xs, CvMat* R, CvMat* t);


}