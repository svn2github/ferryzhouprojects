#ifndef SEQUENCE_IMAGE_FILTER_H_
#define SEQUENCE_IMAGE_FILTER_H_

#include "ImageFilter.h"
#include "SmartImageFilter.h"

#include <vector>

namespace image_filter {

//the filters input/output may not match
//
class SequenceImageFilter : public ImageFilter {
public:
	SequenceImageFilter(void);
	~SequenceImageFilter(void);

public:
	//if no contained filter, return a copy
	IplImage* process(const IplImage* im, IplImage* buffer = NULL);

public:
	//this is reponsible for deleting the input filter
	void addFilter(ImageFilter* filter);

private:
	std::vector<SmartImageFilter*> filters;
};

}
#endif // SEQUENCE_IMAGE_FILTER_H_
