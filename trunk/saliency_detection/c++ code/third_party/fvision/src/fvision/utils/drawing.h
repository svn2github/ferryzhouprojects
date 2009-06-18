#pragma once

#include <vector>
#include <string>

#include <cv.h>
#include <highgui.h>

/**
 * 
 */
namespace fvision {

class PointDrawer;
class LineDrawer;
class TextDrawer;

IplImage* perspectiveWarp(const IplImage* src, const CvMat* H);

IplImage* createSideBySideImage(const IplImage* im_L, const IplImage* im_R);

IplImage* createVerticalSideBySideImage(const IplImage* im_L, const IplImage* im_R);

IplImage* mix(const IplImage* im_L, const IplImage* im_R);

IplImage* mixToRedCyan(const IplImage* im_L, const IplImage* im_R);
void mixToRedCyan(const IplImage* im_L, const IplImage* im_R, IplImage* dstRedCyan);


void showImage(const char* winname, const IplImage* im);

void drawPoints(IplImage* src, const std::vector<CvPoint>& pts, const PointDrawer& pd);
void drawPoints(IplImage* src, const std::vector<CvPoint2D32f>& pts, const PointDrawer& pd);

void drawPointsWithId(IplImage* src, 
					  const std::vector<CvPoint2D32f>& pts, 
					  const std::vector<int>& ids, 
					  const PointDrawer& pd,
					  const TextDrawer& td);

void drawPointsWithId(IplImage* src, 
					  const std::vector<CvPoint>& pts, 
					  const std::vector<int>& ids, 
					  const PointDrawer& pd,
					  const TextDrawer& td);

void drawPointWithId(IplImage* src, 
					 const CvPoint& pt, 
					 int id,
					 const PointDrawer& pd,
					 const TextDrawer& td);

void drawPointWithId(IplImage* src, 
					 const CvPoint2D32f& pt, 
					 int id,
					 const PointDrawer& pd,
					 const TextDrawer& td);

void drawPolygon(IplImage* src, const std::vector<CvPoint2D32f>& pts, const LineDrawer& ld);

void drawLine(IplImage* im, CvMat* line, const LineDrawer& ld);
void drawLines(IplImage* im, const std::vector<CvMat*>& lines, const LineDrawer& ld);


void drawImagedXAxis(IplImage* im, const LineDrawer& ld);
void drawImagedYAxis(IplImage* im, const LineDrawer& ld);
}