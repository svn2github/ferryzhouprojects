#ifndef FILTERED_IMAGE_OUTPUT_HANDLER_H_
#define FILTERED_IMAGE_OUTPUT_HANDLER_H_

#include "ImageOutputHandler.h"

#include <ImageFilter.h>

namespace image_io {

class FilteredImageOutputHandler : public ImageOutputHandler {
public:
	FilteredImageOutputHandler(ImageOutputHandler* outputHandler, image_filter::ImageFilter* filter);
	~FilteredImageOutputHandler(void);

public:
	void write(const IplImage* im) throw(ImageIoException);

private:
	ImageOutputHandler* outputHandler;
	image_filter::ImageFilter* filter;
};


}
#endif // FILTERED_IMAGE_OUTPUT_HANDLER_H_