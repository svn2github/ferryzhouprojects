#include "ImageInputHandlerFactory.h"

#include "impl/PathRegexImageInputHandler.h"

namespace image_io {

ImageInputHandlerFactory::ImageInputHandlerFactory(void)
{
}

ImageInputHandlerFactory::~ImageInputHandlerFactory(void)
{
}

ImageInputHandler* ImageInputHandlerFactory::createPathRegexImageInputHandler(const char* pathRegex, int startIndex, int endIndex, int interval /*= 1*/) {
	return new impl::PathRegexImageInputHandler(pathRegex, startIndex, endIndex, interval);
}

}