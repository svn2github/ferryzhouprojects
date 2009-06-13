#include "SequenceImageFilter.h"

#include "image_filter_utils.h"

namespace image_filter {

SequenceImageFilter::SequenceImageFilter(void)
{
}

SequenceImageFilter::~SequenceImageFilter(void)
{
}

void SequenceImageFilter::addFilter(ImageFilter* filter) {
	assert (filter != NULL);
	filters.push_back(new SmartImageFilter(filter));
}

IplImage* SequenceImageFilter::process(const IplImage* im, IplImage* buffer /*= NULL*/) {
	if (im == NULL) return NULL;
	if (filters.size() == 0) {
		if (buffer == NULL) {
			buffer = cvCloneImage(im);
		} else {
			assert (isImageHeaderEqual(im, buffer));
			cvCopy(im, buffer);
		}
		return buffer;
	}
	if (filters.size() == 1) {
		return filters[0]->getInternalFilter()->process(im, buffer);
	}
	IplImage* prevOutput = filters[0]->process(im);
	for (size_t i = 1; i < filters.size() - 1; i++) {
		prevOutput = filters[i]->process(prevOutput);
	}
	size_t lastIndex = filters.size() - 1;
	buffer = filters[lastIndex]->getInternalFilter()->process(prevOutput, buffer);
	return buffer;
}

}