#ifndef PATH_REGEX_IMAGE_OUTPUT_HANDLER_H_
#define PATH_REGEX_IMAGE_OUTPUT_HANDLER_H_

#include "../ImageOutputHandler.h"

#include <string>

namespace image_io { namespace impl {

using namespace image_io;

//
class PathRegexImageOutputHandler : public ImageOutputHandler {
public:
	PathRegexImageOutputHandler(const std::string& pathRegex, int startIndex, int interval = 1);
	~PathRegexImageOutputHandler(void);

public:
	void write(const IplImage* im) throw(ImageIoException);

private:
	std::string pathRegex;
	int startIndex;
	int interval;

	char* pathBuf;

	int curIndex;

};

}}
#endif // PATH_REGEX_IMAGE_OUTPUT_HANDLER_H_
