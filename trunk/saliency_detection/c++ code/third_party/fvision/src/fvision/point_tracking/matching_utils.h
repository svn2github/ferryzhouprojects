#pragma once

#include <cv.h>
#include <highgui.h>

#include <iostream>
#include <vector>

namespace fvision {

//compute difference between two feature points based on a local window
//type: 0 - gray; 1 - color (BGR); 2 - hs(v) ; 3 - rg(b);
//for gray image, only have type 0;
//for color image, can have all types
//for type 0, assume gray image
//for type 1, 2 and 3, assume BGR, hsv/rgb image and only compute first two channels
float windowMatch(const IplImage* im1, 
				  const IplImage* im2, 
				  const CvPoint& p1, 
				  const CvPoint& p2, 
				  int halfWinSize, 
				  int type = 0);

//compute difference between p1 and a set of points around p2
float* windowNeighborMatch(const IplImage* im1, 
						   const IplImage* im2, 
						   const CvPoint& p1, 
						   const CvPoint& p2, 
						   int halfWinSize, 
						   int halfNeighborWinSize, 
						   int type);

//return points with distance <= maxDistance
std::vector<CvPoint> filterPointsByDistance(const CvPoint& p1, const std::vector<CvPoint>& ps, double maxDistance);

//double pointDistance(const CvPoint& p1, const CvPoint& p2);

}