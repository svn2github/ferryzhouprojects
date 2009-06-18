#ifndef FVISION_IMIO_IMAGE_SEQUENCE_READER_H_
#define FVISION_IMIO_IMAGE_SEQUENCE_READER_H_

#include <cv.h>

namespace fvision {

/**
 * Unified interface for reading bmp, avi and image sequences. based on suffix.
 */
class ImageSequenceReader {
public:
	ImageSequenceReader() {
		path = NULL;
	}

	ImageSequenceReader(const char* path);

	virtual ~ImageSequenceReader(void);

public:
	//examine whether initialization failed
	virtual bool isFailed() = 0;

	//return null if no next frame
	//user is not responsible for deleting the frame
	virtual IplImage* nextFrame() = 0;

	virtual void close() = 0;

	//get filename (not the whole path) of the current frame
	virtual const char* getFilename() const = 0;

	virtual const char* getPath() const { return path; }

private:
	char* path;

};

} 
#endif // FVISION_IMIO_IMAGE_SEQUENCE_READER_H_