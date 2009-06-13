#ifndef IMAGE_FILTER_H_
#define IMAGE_FILTER_H_

#include <cv.h>

#include <exception>

namespace image_filter {

class ImageFilterTypeNotMatchException : public std::exception {
};

class ImageFilter {
public:
	virtual ~ImageFilter(void) {}
	//if input NULL, return NULL
	//the input buffer may not match filter's output, in this case, throw an exception
	virtual IplImage* process(const IplImage* im, IplImage* buffer = NULL) = 0;
};

}

#endif // IMAGE_FILTER_H_
