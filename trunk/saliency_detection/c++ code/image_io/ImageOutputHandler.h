#ifndef IMAGE_OUTPUT_HANDLER_H_
#define IMAGE_OUTPUT_HANDLER_H_

#include "ImageIoException.h"

#include <cv.h>

namespace image_io {

class ImageOutputHandler {
public:
	virtual ~ImageOutputHandler(void) {}
	virtual void write(const IplImage* im) throw(ImageIoException) = 0;
};

}
#endif // IMAGE_OUTPUT_HANDLER_H_
