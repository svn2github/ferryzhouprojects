#pragma once

#include <iostream>
#include <vector>

#include <cassert>

#include "cv.h"
#include "highgui.h"

#include "CvMatUtil.h"

using namespace std;
using namespace ferry::cv_mat;

namespace ferry {
	namespace cv_geometry {

IplImage* create_correspondences_image(IplImage* image, const vector<CvPoint>& x1s, const vector<CvPoint>& x2s);

IplImage* create_numbers_correspondences_image(IplImage* image, const vector<CvPoint>& x1s, const vector<CvPoint>& x2s, CvScalar color = CV_RGB(255, 0, 0));

void drawNumbers(IplImage* image, const vector<CvPoint>& x1s, CvScalar color);

void drawLines(IplImage* image, const vector<CvPoint>& x1s, const vector<CvPoint>& x2s, CvScalar color);

void draw_numbers_correspondences_image(char* winname, IplImage* image, const vector<CvPoint>& x1s, const vector<CvPoint>& x2s, CvScalar color = CV_RGB(255, 0, 0));

void draw_correspondences_image(char* winname, IplImage* image, const vector<CvPoint>& x1s, const vector<CvPoint>& x2s);

void draw_correspondences_image(char* winname, IplImage* image, const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s);

void save_correspondences(char* filename, IplImage* image, const vector<CvPoint>& x1s, const vector<CvPoint>& x2s);

IplImage* create_correspondences_image(IplImage* image1, IplImage* image2, const vector<CvPoint>& x1s, const vector<CvPoint>& x2s);

void draw_correspondences_image(char* winname, IplImage* image1, IplImage* image2, const vector<CvPoint>& x1s, const vector<CvPoint>& x2s);

void save_correspondences(char* filename, IplImage* image1, IplImage* image2, const vector<CvPoint>& x1s, const vector<CvPoint>& x2s);

void drawCornersImage(char* winname, IplImage* image, const vector<CvPoint>& corners);

IplImage* createGroundObstaclePointsImage(IplImage* image, const vector<CvPoint>& gxs, const vector<CvPoint>& oxs, CvScalar g_color = CV_RGB(0, 255, 0), CvScalar o_color = CV_RGB(255, 0, 0));

void drawGroundObstaclePointsImage(char* winname, IplImage* image, const vector<CvPoint>& gxs, const vector<CvPoint>& oxs, CvScalar g_color = CV_RGB(0, 255, 0), CvScalar o_color = CV_RGB(255, 0, 0));

IplImage* createCornersImage(IplImage* image, const vector<CvPoint>& corners);

void saveCorners(char* filename, IplImage* image, const vector<CvPoint>& corners, CvScalar color);

void drawCrosses(IplImage* cross, const vector<CvPoint>& pts, CvScalar color, int thickness = 1);

void drawCross(char* winname, IplImage* image, CvPoint pt);

void drawCross(IplImage* cross, CvPoint pt);

void drawCross(IplImage* cross, CvPoint pt, CvScalar color, int thickness = 1);


//==================
void drawEpipoleLines(IplImage* im1, IplImage* im2, CvMat* F, CvScalar color = CV_RGB(255, 255, 255));

void drawEpipoleLine(IplImage* im1, IplImage* im2, CvMat* F, CvMat* e1, CvPoint2D32f p1, CvScalar color = CV_RGB(255, 255, 255));

void drawLine(IplImage* im, CvMat* line, CvScalar color = CV_RGB(255, 255, 255), int thickness = 1);
void drawLines(IplImage* im, vector<CvMat*> lines, CvScalar color = CV_RGB(255, 255, 255), int thickness = 1);

//==================
	}
}