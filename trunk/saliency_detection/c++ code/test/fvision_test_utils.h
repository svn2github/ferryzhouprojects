#pragma once

#include <cv.h>
#include <vector>

namespace fvision {

void assertMatEquals(const CvMat* src, const CvMat* dst, double delta = FLT_EPSILON);
void assertMatEqualsMessage(const char* message, const CvMat* src, const CvMat* dst, double delta = FLT_EPSILON);

void assertHomogeneousMatEqualsMessage(const char* message, const CvMat* src, const CvMat* dst, double delta = FLT_EPSILON);

void assertPointEquals(CvPoint3D32f p1, CvPoint3D32f p2, double value = FLT_EPSILON);
void assertPointsEqual(const std::vector<CvPoint3D32f>& p1s, const std::vector<CvPoint3D32f>& p2s, double value = FLT_EPSILON);

void assertPointEquals(CvPoint3D64f p1, CvPoint3D64f p2, double value = FLT_EPSILON);
void assertPointEqualsMessage(const char* message, CvPoint3D64f p1, CvPoint3D64f p2, double value = FLT_EPSILON);

void assertPointEquals(CvPoint2D32f p1, CvPoint2D32f p2, double value = FLT_EPSILON);
void assertPointsEqual(const std::vector<CvPoint2D32f>& p1s, const std::vector<CvPoint2D32f>& p2s, double value = FLT_EPSILON);

}
