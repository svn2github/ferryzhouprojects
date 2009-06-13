#ifndef SMOOTH_IMAGE_FILTER_H_
#define SMOOTH_IMAGE_FILTER_H_

#include "ImageFilter.h"

namespace image_filter {

class SmoothImageFilter : public ImageFilter {
private:
	SmoothImageFilter(void);
public:
	~SmoothImageFilter(void);

public:
	//see opencv function cvSmooth
	static SmoothImageFilter* create(
		int smoothType = CV_GAUSSIAN, 
		int param1 = 3, 
		int param2 = 0, 
		double param3 = 0, 
		double param4 = 0);

public:
	IplImage* process(const IplImage* im, IplImage* buffer = NULL);

private:
	int smoothType;
	int param1;
	int param2;
	double param3;
	double param4;
};

}

#endif // SMOOTH_IMAGE_FILTER_H_
