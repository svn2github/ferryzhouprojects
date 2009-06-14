#ifndef WINDOW_IMAGE_OUTPUT_HANDLER_H_
#define WINDOW_IMAGE_OUTPUT_HANDLER_H_

#include "../ImageOutputHandler.h"

#include <string>

namespace image_io { namespace impl {

using namespace image_io;

class WindowImageOutputHandler : public ImageOutputHandler {
public:
	WindowImageOutputHandler(const std::string& windowName);
	~WindowImageOutputHandler(void);

public:
	void write(const IplImage* im) throw(ImageIoException);

private:
	std::string windowName;
};

}}
#endif // WINDOW_IMAGE_OUTPUT_HANDLER_H_
