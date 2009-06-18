#ifndef FVISION_BLOCK_CORNER_DETECTOR_H_
#define FVISION_BLOCK_CORNER_DETECTOR_H_

#include "OpenCVCornerDetector.h"

namespace fvision {

class BlockCornerDetector : public CornerDetector {
public:
	BlockCornerDetector(int nxslice, int nyslice, int corners_count, double quality_level, double min_distance) {
		this->nxslice = nxslice;
		this->nyslice = nyslice;
		this->corners_count = corners_count;
		this->quality_level = quality_level;
		this->min_distance = min_distance;
	}
	~BlockCornerDetector(void);

public:	
	void compute(const IplImage* src, std::vector<CvPoint2D32f>& corners);

private:
	void addCorners(const std::vector<CvPoint2D32f>& block_corners, int x, int y, std::vector<CvPoint2D32f>& corners);

private:
	int nxslice;
	int nyslice;

	int corners_count;
	double quality_level;
	double min_distance;

};

}

#endif // FVISION_BLOCK_CORNER_DETECTOR_H_