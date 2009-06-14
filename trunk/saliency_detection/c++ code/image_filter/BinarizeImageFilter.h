#ifndef BINARIZE_IMAGE_FILTER_H_
#define BINARIZE_IMAGE_FILTER_H_

#include "ImageFilter.h"

namespace image_filter {

class BinarizeImageFilter {
private:
	BinarizeImageFilter(void);
public:
	~BinarizeImageFilter(void);
public:
	//see cvCmpS
	static BinarizeImageFilter* create(double threshold, int cmpOp = CV_CMP_GT);

public:
	//the buffer should be a gray scale image and has same size with the original image
	//the output will be 0 and 255
	IplImage* process(const IplImage* im, IplImage* buffer = NULL);

private:
	double threshold;
	int cmpOp;
};

}
#endif // BINARIZE_IMAGE_FILTER_H_
