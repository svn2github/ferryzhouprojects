#ifndef PATH_REGEX_IMAGE_INPUT_HANDLER_H_
#define PATH_REGEX_IMAGE_INPUT_HANDLER_H_

#include "../ImageInputHandler.h"

#include <string>

namespace image_io { namespace impl {

using namespace image_io;
//
class PathRegexImageInputHandler : public ImageInputHandler {
public:
	PathRegexImageInputHandler(const std::string& pathRegex, int startIndex, int endIndex, int interval = 1);
	~PathRegexImageInputHandler(void);
public:
	IplImage* read();

private:
	std::string pathRegex;
	int startIndex;
	int endIndex;
	int interval;

	char* pathBuf;

	int curIndex;

	IplImage* lastImagePointer;
};

}}
#endif // PATH_REGEX_IMAGE_INPUT_HANDLER_H_
