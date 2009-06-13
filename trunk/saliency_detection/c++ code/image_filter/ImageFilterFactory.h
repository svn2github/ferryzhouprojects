#pragma once

#include "ImageFilter.h"

#include <string>

#include <exception>
#include <vector>

namespace image_filter {

//not implemented yet
class ImageFilterFactory {
public:
	ImageFilterFactory(void);
	~ImageFilterFactory(void);
public:
	class Exception : public std::exception {

	};
public:
	ImageFilter* create(const std::string& filterString);
	ImageFilter* createSequenceFilters(const std::vector<std::string>& filterStrings);
};

}