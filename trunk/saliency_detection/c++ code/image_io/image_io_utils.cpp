#include "image_io_utils.h"

#include <highgui.h>

namespace image_io {

void run(ImageInputHandler* inputHandler, ImageOutputHandler* outputHandler, int waitTime /*= -1*/) throw(ImageIoException) {
	IplImage* im = NULL;
	while ((im = inputHandler->read()) != NULL) {
		outputHandler->write(im);
		if (waitTime > 0) cvWaitKey(waitTime);
	}
}

}