#pragma once

#include <iostream>
#include <cmath>

#include "FeatureDistanceCalculator.h"

using namespace std;

namespace ferry {
	namespace feature_tracking {

class SimpleWindowSSDFDC : public FeatureDistanceCalculator
{
public:
	SimpleWindowSSDFDC(int halfWinSize = 3, int rangeHalfWinSize = 3, int maxDistance = 30) {
		this->maxDistance = maxDistance;
		this->halfWinSize = halfWinSize;
		this->rangeHalfWinSize = rangeHalfWinSize;
	}

public:
	float compute(IplImage* im1, IplImage* im2, const CvPoint& p1, const CvPoint& p2) {
		CvPoint minp2;
		return compute(im1, im2, p1, p2, minp2);
	}

	float compute(IplImage* im1, IplImage* im2, const CvPoint& p1, const CvPoint& p2, CvPoint& minp2) {
		float minssd = 100000.0;
		//CvPoint minp2;
		for (int i = -rangeHalfWinSize; i <= rangeHalfWinSize; i++) {
			for (int j = -rangeHalfWinSize; j <= rangeHalfWinSize; j++) {
				//cout<<"p1: "<<p1.x<<", "<<p1.y<<"........."<<endl;
				//cout<<"p2: "<<p2.x + i<<", "<<p2.y + j<<"............"<<endl;
				float ssd = computessd(im1, im2, p1, cvPoint(p2.x + i, p2.y + j));
				//cout<<"ssd: "<<ssd<<endl;
				if (ssd < minssd)  {
					minssd = ssd;
					minp2.x = p2.x + i;
					minp2.y = p2.y + j;
				}
			}
		}
		if (minssd > maxDistance) return MAX_DISTANCE;
		return minssd;
	}

	float computessd(IplImage* im1, IplImage* im2, const CvPoint& p1, const CvPoint& p2) {

		uchar* data1 = (uchar*)im1->imageData;
		uchar* data2 = (uchar*)im2->imageData;
		CvSize size1 = cvGetSize(im1);
		CvSize size2 = cvGetSize(im2);
		float sum = 0.0;
		int num = 0;
		for (int i = - halfWinSize; i <= halfWinSize; i++) {
			int x1 = p1.x + i;
			int x2 = p2.x + i;
			if (x1 < 0 || x1 >= size1.width) continue;
			if (x2 < 0 || x2 >= size2.width) continue;
			for (int j = - halfWinSize; j <= halfWinSize; j++) {
				int y1 = p1.y + j;
				int y2 = p2.y + j;
				if (y1 < 0 || y1 >= size1.height) continue;
				if (y2 < 0 || y2 >= size2.height) continue;
				//cout<<"p1: "<<x1<<" , "<<y1<<" = "<<cvGetReal2D(im1, y1, x1)<<endl;
				//cout<<"p2: "<<x2<<" , "<<y2<<" = "<<cvGetReal2D(im2, y2, x2)<<endl;
				//float d = cvGetReal2D(im1, y1, x1) - cvGetReal2D(im2, y2, x2);
				float d = (float)(data1[y1 * im1->widthStep + x1] - data2[y2 * im2->widthStep + x2]);
				//sum += d * d;
				sum += abs(d);
				//cout<<"d * d"<<d*d<<endl;
				//cout<<"d"<<d<<endl;
				num++;
			}
		}
		//cout<<"num: "<<num<<endl;
		if (num != 0) return sum / num;
		else return 0;
	}


private:
	int maxDistance;
	int halfWinSize;
	int rangeHalfWinSize;

};

	}
}