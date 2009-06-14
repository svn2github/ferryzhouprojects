#include "SequenceImageOutputHandler.h"

namespace image_io {

SequenceImageOutputHandler::SequenceImageOutputHandler(void)
{
}

SequenceImageOutputHandler::~SequenceImageOutputHandler(void) {
	for (size_t i = 0; i < outputHandlers.size(); i++) {
		delete outputHandlers[i];
		outputHandlers[i] = NULL;
	}
}

void SequenceImageOutputHandler::addImageOutputHandler(ImageOutputHandler* outputHandler) {
	outputHandlers.push_back(outputHandler);
}

void SequenceImageOutputHandler::write(const IplImage* im) throw(ImageIoException) {
	for (size_t i = 0; i < outputHandlers.size(); i++) {
		outputHandlers[i]->write(im);
	}
}
}