#ifndef IMAGE_INPUT_HANDLER_FACTORY_H_
#define IMAGE_INPUT_HANDLER_FACTORY_H_

#include "ImageInputHandler.h"

namespace image_io {

class ImageInputHandlerFactory {
public:
	ImageInputHandlerFactory(void);
	~ImageInputHandlerFactory(void);

public:
	ImageInputHandler* createPathRegexImageInputHandler(const char* pathRegex, int startIndex, int endIndex, int interval = 1);

	//ImageInputHandler* createAviImageInputHandler(const char* aviPath);
};

}
#endif // IMAGE_INPUT_HANDLER_FACTORY_H_
