#include "FilteredImageOutputHandler.h"

#include <SmartImageFilter.h>

namespace image_io {

FilteredImageOutputHandler::FilteredImageOutputHandler(ImageOutputHandler* outputHandler, image_filter::ImageFilter* filter) {
	this->outputHandler = outputHandler;
	this->filter = new image_filter::SmartImageFilter(filter);
}

FilteredImageOutputHandler::~FilteredImageOutputHandler(void) {
	delete filter;
	delete outputHandler;
}

void FilteredImageOutputHandler::write(const IplImage* im) throw(ImageIoException) {
	outputHandler->write(filter->process(im));
}
}