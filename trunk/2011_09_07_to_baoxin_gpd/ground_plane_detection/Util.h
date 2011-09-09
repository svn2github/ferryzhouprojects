#pragma once

#include <cv.h>
#include <highgui.h>
#include "RhoThetaLine.h"

#include <vector>

using namespace std;

IplImage* compare(IplImage* im1, IplImage* im2, CvMat* H);

void drawLines(IplImage* im, vector<RhoThetaLine>& lines);

CvMat* getLine(CvPoint2D32f p1, CvPoint2D32f p2);
CvMat* getLine(CvPoint p1, CvPoint p2);

vector<CvMat*> linesToMats(vector<RhoThetaLine>& vlines);

CvPoint3D32f normalize(const CvPoint3D32f& p);

CvPoint3D32f addPoint3D(const CvPoint3D32f& p1, const CvPoint3D32f& p2);
