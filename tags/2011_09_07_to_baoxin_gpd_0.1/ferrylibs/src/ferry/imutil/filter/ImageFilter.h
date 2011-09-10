#pragma once

#include "cv.h"

namespace ferry { namespace imutil { namespace filter {

class ImageFilter
{
public:
	ImageFilter(void) {}
	~ImageFilter(void) {}

public:
	virtual IplImage* process(IplImage* src, IplImage* dst) = 0;

	//process without buffer
	//remember to delete the returned image
	IplImage* process(IplImage* src) {
		IplImage* dst = cvCloneImage(src);
		return process(src, dst);
	}
};

} } }
