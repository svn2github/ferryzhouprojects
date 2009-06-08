#pragma once

#include <cv.h>

class IndexedImageBufferProcessor {
public:
	virtual ~IndexedImageBufferProcessor(void) {}
	//if return NULL, means no more image
	virtual IplImage* queryImage(int index) const = 0;
	virtual bool bufferNext() = 0;
};
