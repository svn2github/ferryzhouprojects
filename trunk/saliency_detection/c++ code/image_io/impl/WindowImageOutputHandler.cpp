#include "WindowImageOutputHandler.h"

#include <highgui.h>

using namespace image_io;

namespace image_io { namespace impl {

WindowImageOutputHandler::WindowImageOutputHandler(const std::string& windowName) {
	assert (windowName.size() > 0 && windowName.size() < 100);
	this->windowName = windowName;
	cvNamedWindow(windowName.c_str());
}

WindowImageOutputHandler::~WindowImageOutputHandler(void) {
	cvDestroyWindow(windowName.c_str());
}

void WindowImageOutputHandler::write(const IplImage* im) throw(ImageIoException) {
	cvShowImage(windowName.c_str(), im);	
}

}}