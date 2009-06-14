#include "BinarizeImageFilter.h"

namespace image_filter {

BinarizeImageFilter::BinarizeImageFilter(void)
{
}

BinarizeImageFilter::~BinarizeImageFilter(void)
{
}

BinarizeImageFilter* BinarizeImageFilter::create(double threshold, int cmpOp) {
	BinarizeImageFilter* filter = new BinarizeImageFilter();
	filter->threshold = threshold;
	filter->cmpOp = cmpOp;
	return filter;
}

IplImage* BinarizeImageFilter::process(const IplImage* im, IplImage* buffer /*= NULL*/) {
	if (im == NULL) return NULL;
	assert (im->nChannels == 1);
	if (buffer == NULL) {
		buffer = cvCreateImage(cvGetSize(im), 8, 1);
	} else {
		assert(buffer->nChannels = 1 && buffer->width == im->width && buffer->height == im->height);
	}
	cvCmpS(im, threshold, buffer, cmpOp);
	return buffer;
}
}