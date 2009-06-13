#include "SmartImageFilter.h"

namespace image_filter {

SmartImageFilter::SmartImageFilter(ImageFilter* filter) {
	assert (filter != NULL);
	this->filter = filter;
	outputBuffer = NULL;
}

SmartImageFilter::~SmartImageFilter(void) {
	delete filter;
	if (outputBuffer != NULL) cvReleaseImage(&outputBuffer);
}

//if the input type do not change, then buffer do not need change
//otherwise, change buffer
IplImage* SmartImageFilter::process(const IplImage* im, IplImage* buffer /*= NULL*/) {
	if (im == NULL) return NULL;
	ImageTypeInfo newTypeInfo = ImageTypeInfo::getTypeInfo(im);
	if (!(lastInputTypeInfo == newTypeInfo)) {
		cvReleaseImage(&outputBuffer);
		outputBuffer = NULL;
	}
	outputBuffer = filter->process(im, outputBuffer);
	return outputBuffer;
}

}