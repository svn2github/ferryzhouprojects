#pragma once

#include <cv.h>

#include <vector>
#include <iostream>

namespace fvision {

std::ostream& operator<<(std::ostream& os, const std::vector<CvPoint2D32f>& pts);
std::istream& operator>>(std::istream& is, std::vector<CvPoint2D32f>& pts);

std::ostream& operator<<(std::ostream& os, const CvPoint2D32f& pt);
std::istream& operator>>(std::istream& is, CvPoint2D32f& pt);

std::ostream& operator<<(std::ostream& os, const CvPoint2D64f& pt);
std::istream& operator>>(std::istream& is, CvPoint2D64f& pt);

std::ostream& operator<<(std::ostream& os, const CvPoint3D32f& pt);
std::istream& operator>>(std::istream& is, CvPoint3D32f& pt);

std::ostream& operator<<(std::ostream& os, const CvPoint3D64f& pt);
std::istream& operator>>(std::istream& is, CvPoint3D64f& pt);

std::ostream& operator<<(std::ostream& os, const std::vector<CvPoint3D32f>& pts);
std::istream& operator>>(std::istream& is, std::vector<CvPoint3D32f>& pts);

std::ostream& operator<<(std::ostream& os, const CvPoint3D64f& pt);
std::istream& operator>>(std::istream& is, CvPoint3D64f& pt);

std::ostream& operator<<(std::ostream& os, const std::vector<CvPoint>& pts);
std::istream& operator>>(std::istream& is, std::vector<CvPoint>& pts);

std::ostream& operator<<(std::ostream& os, const CvPoint& pt);
std::istream& operator>>(std::istream& is, CvPoint& pt);

std::ostream& operator<<(std::ostream& os, const CvMat* M);
std::istream& operator>>(std::istream& is, CvMat** pM);

CvMat* loadMat(const char* filename);

void load(const char* filename, std::vector<CvMat*>& ms);

void save(const CvMat* M, const char* filename);
void save(const std::vector<CvMat*>& ms, const char* filename);
void save(const std::vector<double>& xs, const char* filename);
void save(const std::vector<float>& xs, const char* filename);
void save(const std::vector<CvPoint3D32f>& pts, const char* filename);

void saveMatWithNoHeader(const char* filename, const CvMat* M);

std::ostream& operator<<(std::ostream& os, const std::vector<double>& vs);

void toMatlabFile(std::ostream& os, const CvMat* M, const char* varname);
}