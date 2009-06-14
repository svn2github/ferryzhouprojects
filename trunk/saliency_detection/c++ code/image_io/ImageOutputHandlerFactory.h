#ifndef IMAGE_OUTPUT_HANDLER_FACTORY_H_
#define IMAGE_OUTPUT_HANDLER_FACTORY_H_

#include "ImageOutputHandler.h"

namespace image_io {

class ImageOutputHandlerFactory {
public:
	ImageOutputHandlerFactory(void);
	~ImageOutputHandlerFactory(void);

public:
	ImageOutputHandler* createPathRegexImageOutputHandler(const char* pathRegex, int startIndex, int interval = 1);
	ImageOutputHandler* createWindowImageOutputHandler(const char* windowName);
	//ImageOutputHandler* createAviImageInputHandler(const char* aviPath);
};

}
#endif // IMAGE_OUTPUT_HANDLER_FACTORY_H_
