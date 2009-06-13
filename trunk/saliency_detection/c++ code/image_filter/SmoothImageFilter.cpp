#include "SmoothImageFilter.h"

#include "image_filter_utils.h"

namespace image_filter {

SmoothImageFilter::SmoothImageFilter(void)
{
}

SmoothImageFilter::~SmoothImageFilter(void)
{
}

SmoothImageFilter* SmoothImageFilter::create(int smoothType /*= CV_GAUSSIAN*/, int param1 /*= 3*/, int param2 /*= 0*/, double param3 /*= 0*/, double param4 /*= 0*/) {
	SmoothImageFilter* filter = new SmoothImageFilter();
	filter->smoothType = smoothType;
	filter->param1 = param1;
	filter->param2 = param2;
	filter->param3 = param3;
	filter->param4 = param4;
	return filter;
}

IplImage* SmoothImageFilter::process(const IplImage* im, IplImage* buffer /*= NULL*/) {
	if (im == NULL) return NULL;
	if (buffer == NULL) buffer = cvCloneImage(im);
	else assert (image_filter::isImageHeaderEqual(im, buffer));
	cvSmooth(im, buffer, smoothType, param1, param2, param3, param4);
	return buffer;
}

}