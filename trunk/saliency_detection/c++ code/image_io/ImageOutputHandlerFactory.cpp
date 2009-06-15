#include "ImageOutputHandlerFactory.h"

#include "impl/PathRegexImageOutputHandler.h"
#include "impl/WindowImageOutputHandler.h"

namespace image_io {

ImageOutputHandlerFactory::ImageOutputHandlerFactory(void)
{
}

ImageOutputHandlerFactory::~ImageOutputHandlerFactory(void)
{
}

ImageOutputHandler* ImageOutputHandlerFactory::createPathRegexImageOutputHandler(const char* pathRegex, int startIndex, int interval /*= 1*/) {
	return new impl::PathRegexImageOutputHandler(pathRegex, startIndex, interval);
}

ImageOutputHandler* ImageOutputHandlerFactory::createWindowImageOutputHandler(const char* windowName, int waitTime) {
	return new impl::WindowImageOutputHandler(windowName, waitTime);
}
}