#ifndef SEQUENCE_IMAGE_OUTPUT_HANDLER_H_
#define SEQUENCE_IMAGE_OUTPUT_HANDLER_H_

#include "ImageOutputHandler.h"

#include <vector>

namespace image_io {

class SequenceImageOutputHandler : public ImageOutputHandler {
public:
	SequenceImageOutputHandler(void);
	~SequenceImageOutputHandler(void);
public:
	//responsible for deleting outputHandler
	void addImageOutputHandler(ImageOutputHandler* outputHandler);
	void write(const IplImage* im) throw(ImageIoException);

private:
	std::vector<ImageOutputHandler*> outputHandlers;
};

}
#endif // SEQUENCE_IMAGE_OUTPUT_HANDLER_H_