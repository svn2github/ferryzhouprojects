#include "FilteredImageInputHandler.h"

#include <SmartImageFilter.h>

namespace image_io {

FilteredImageInputHandler::FilteredImageInputHandler(ImageInputHandler* inputHandler, image_filter::ImageFilter* filter) {
	this->inputHandler = inputHandler;
	this->filter = new image_filter::SmartImageFilter(filter);
}

FilteredImageInputHandler::~FilteredImageInputHandler(void) {
	delete filter;
	delete inputHandler;
}

IplImage* FilteredImageInputHandler::read() {
	return filter->process(inputHandler->read());
}

}