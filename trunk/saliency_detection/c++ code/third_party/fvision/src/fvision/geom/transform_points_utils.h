#pragma once

#include <cv.h>

#include <vector>

#include <fvision/utils/pointpair.h>

namespace fvision {
//x' = H * x
CvPoint2D32f transformPoint(const CvMat* H, const CvPoint2D32f& p);

std::vector<CvPoint2D32f> transformPoints(const CvMat* H, const std::vector<CvPoint2D32f>& xs);

std::vector<CvPoint2D32f> normalizePointsWithCameraInternalMatrix(const CvMat* K, const std::vector<CvPoint2D32f>& xs);

PointPairs transformPointPairs(const CvMat* H, const PointPairs& pps);

}