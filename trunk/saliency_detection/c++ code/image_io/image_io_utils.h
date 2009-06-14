#ifndef IMAGE_IO_UTILS_H_
#define IMAGE_IO_UTILS_H_

#include "ImageInputHandler.h"
#include "ImageOutputHandler.h"

#include <vector>

namespace image_io {

//read from input, write to output, wait after each write
//if waitTime <= 0, do not wait, otherwise, use cvWaitKey(waitTime)
void run(ImageInputHandler* inputHandler, ImageOutputHandler* outputHandler, int waitTime = -1) throw(ImageIoException);

}

#endif // IMAGE_IO_UTILS_H_
