#pragma once

#include "ImageInputHandler.h"

#include <SmartImageFilter.h>

namespace image_io {

class FilteredImageInputHandler : public ImageInputHandler {
public:
	FilteredImageInputHandler(ImageInputHandler* inputHandler, image_filter::ImageFilter* filter);
	~FilteredImageInputHandler(void);
public:
	IplImage* read();

private:
	ImageInputHandler* inputHandler;
	image_filter::ImageFilter* filter;
};

}