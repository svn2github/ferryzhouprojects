#include "ResizeImageFilter.h"

namespace image_filter {

ResizeImageFilter::ResizeImageFilter(void) {
	ratio = -1.0;
	newSize = cvSize(-1, -1);
	useTargetBufferSize = false;
}

ResizeImageFilter::~ResizeImageFilter(void) {
}

IplImage* ResizeImageFilter::process(const IplImage* im, IplImage* buffer /*= NULL*/) {
	if (im == NULL) return NULL;
	if (useTargetBufferSize) {
		assert (buffer != NULL);
	} else {
		determineNewSize(cvGetSize(im));
		if (buffer != NULL) {
			assert (buffer->width == newSize.width && buffer->height == newSize.height);
		} else {
			buffer = cvCreateImage(finalSize, im->depth, im->nChannels);
		}
	}
	cvResize(im, buffer);
	return buffer;
}

void ResizeImageFilter::determineNewSize(const CvSize& inputImageSize) {
	if (ratio > 0) {
		finalSize.width = (int)(ratio * inputImageSize.width);
		finalSize.height = (int)(ratio * inputImageSize.height);
	} else {
		finalSize = newSize;
	}
}

ResizeImageFilter* ResizeImageFilter::createWithNewSize(const CvSize& size) {
	assert (size.width > 0 && size.height > 0);
	ResizeImageFilter* filter = new ResizeImageFilter();
	filter->newSize = size;
	return filter;
}

ResizeImageFilter* ResizeImageFilter::createWithNewRatio(double ratio) {
	assert (ratio > 0);
	ResizeImageFilter* filter = new ResizeImageFilter();
	filter->ratio = ratio;
	return filter;
}

}