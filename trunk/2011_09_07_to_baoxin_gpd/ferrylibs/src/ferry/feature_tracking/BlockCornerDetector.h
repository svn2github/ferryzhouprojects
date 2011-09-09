#pragma once

#include "OpenCVCornerDetector.h"

namespace ferry {
	namespace feature_tracking {

class BlockCornerDetector : public CornerDetector
{
public:
	BlockCornerDetector(int block_size, int corners_count, double quality_level, double min_distance) {
		this->block_size = block_size;
		this->corners_count = corners_count;
		this->quality_level = quality_level;
		this->min_distance = min_distance;
	}
	~BlockCornerDetector(void);

public:
	
	void compute(IplImage* src, vector<CvPoint>& corners) {
		OpenCVCornerDetector* pcd = new OpenCVCornerDetector(corners_count, quality_level, min_distance);
		CvSize size = cvGetSize(src);
		for (int i = 0; i < size.width / block_size; i++) {
			for (int j = 0; j < size.height / block_size; j++) {
				vector<CvPoint> block_corners;
				cvSetImageROI(src, cvRect(i * block_size, j * block_size, block_size, block_size));
				pcd->compute(src, block_corners);
				addCorners(block_corners, i * block_size, j * block_size, corners);
			}
		}
		cvSetImageROI(src, cvRect(0, 0, size.width, size.height));
		delete pcd;
	}

	void addCorners(const vector<CvPoint>& block_corners, int x, int y, vector<CvPoint>& corners) {
		for (int i = 0; i < (int)block_corners.size(); i++) {
			corners.push_back(cvPoint(block_corners[i].x + x, block_corners[i].y + y));
		}
	}

private:
	int block_size;

	int corners_count;
	double quality_level;
	double min_distance;

};

	}
}