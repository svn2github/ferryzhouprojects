#ifndef RESIZE_IMAGE_FILTER_H_
#define RESIZE_IMAGE_FILTER_H_

#include "ImageFilter.h"

namespace image_filter {

class ResizeImageFilter : public ImageFilter {
private:
	ResizeImageFilter(void);
public:
	~ResizeImageFilter(void);
public:
	static ResizeImageFilter* createWithNewSize(const CvSize& size);
	static ResizeImageFilter* createWithNewRatio(double ratio);

	//the new input size will be determined by target buffer
	//in this case, the input buffer should not be NULL
	static ResizeImageFilter* createWithTargetBuffer();

public:
	IplImage* process(const IplImage* im, IplImage* buffer = NULL);

private:
	void determineNewSize(const CvSize& inputImageSize);

private:
	double ratio;
	CvSize newSize;
	bool useTargetBufferSize;

	CvSize finalSize;
};

}

#endif // RESIZE_IMAGE_FILTER_H_
