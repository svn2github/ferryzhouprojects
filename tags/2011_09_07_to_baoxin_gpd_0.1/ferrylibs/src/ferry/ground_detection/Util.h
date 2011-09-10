#pragma once

#include <cv.h>
#include <highgui.h>
#include "VerticalLineDetector.h"

IplImage* compare(IplImage* im1, IplImage* im2, CvMat* H);

void drawLines(IplImage* im, vector<RhoThetaLine>& lines);