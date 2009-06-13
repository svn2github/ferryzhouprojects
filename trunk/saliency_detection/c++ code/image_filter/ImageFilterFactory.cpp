#include "ImageFilterFactory.h"

namespace image_filter {

ImageFilterFactory::ImageFilterFactory(void)
{
}

ImageFilterFactory::~ImageFilterFactory(void)
{
}

ImageFilter* ImageFilterFactory::create(const std::string& filterString) {
	return NULL;
}

ImageFilter* ImageFilterFactory::createSequenceFilters(const std::vector<std::string>& filterStrings) {
	return NULL;
}
}