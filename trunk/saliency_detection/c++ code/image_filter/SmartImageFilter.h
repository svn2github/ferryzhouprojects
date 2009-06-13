#ifndef SMART_IMAGE_FILTER_H_
#define SMART_IMAGE_FILTER_H_

#include "ImageFilter.h"
#include "ImageTypeInfo.h"

namespace image_filter {

//automatic memory management
//the input buffer is ignored
class SmartImageFilter : public ImageFilter {
public:
	//responsible for deleting filter
	SmartImageFilter(ImageFilter* filter);
	~SmartImageFilter(void);

public:
	//assert buffer == NULL
	IplImage* process(const IplImage* im, IplImage* buffer = NULL);

	ImageFilter* getInternalFilter() const { return filter; }

private:
	ImageFilter* filter;
	IplImage* outputBuffer;

	ImageTypeInfo lastInputTypeInfo;
};

}
#endif // SMART_IMAGE_FILTER_H_
