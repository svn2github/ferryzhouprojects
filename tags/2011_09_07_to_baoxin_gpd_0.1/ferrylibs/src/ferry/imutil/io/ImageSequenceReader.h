#pragma once

#include "cv.h"
#include "highgui.h"

namespace ferry { namespace imutil {
	namespace io {
/*!
* Unified interface for reading bmp, avi and image sequences. based on suffix.
*/
class ImageSequenceReader
{
public:
	ImageSequenceReader(const char* path) {
		this->path = new char[strlen(path) + 1];
		strcpy(this->path, path);
	}

	virtual ~ImageSequenceReader(void) {
		if (path != NULL) {
			delete path;
			path = NULL;
		}
	}

public:
	//examine whether initialization failed
	virtual bool isFailed() = 0;

	//return null if no next frame
	virtual IplImage* nextFrame() = 0;

	virtual void close() = 0;

	virtual char* getFilename() = 0;

	char* getPath() {
		return path;
	}

private:
	char* path;

};

} } }