#ifndef IMAGE_INPUT_HANDLER_H_
#define IMAGE_INPUT_HANDLER_H_

#include <cv.h>

namespace image_io {

class ImageInputHandler {
public:
	virtual ~ImageInputHandler(void) {}
	//the cliend should not delete the returned pointer
	//return NULL if no next
	virtual IplImage* read() = 0;
};

}

#endif // IMAGE_INPUT_HANDLER_H_
