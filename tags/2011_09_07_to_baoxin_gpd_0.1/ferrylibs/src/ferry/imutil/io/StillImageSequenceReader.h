#pragma once

#include <ferry/util/StringUtil.h>

#include "ImageSequenceReader.h"

using namespace ferry::util;

namespace ferry { namespace imutil {
	namespace io {

/*!
* read single still image
*/
class StillImageSequenceReader : public ImageSequenceReader {
public:
	StillImageSequenceReader(const char* path) : ImageSequenceReader(path) {
		image = cvLoadImage(path, 1);
		if (image == NULL) failed = true;
		else failed = false;
		picked = false;

		string filenameBuf;
		sprintf(this->filename, "%s", getFileName(path, filenameBuf));
	}

	~StillImageSequenceReader(void) {
		if (image != NULL) cvReleaseImage(&image);
	}

public:
	bool isFailed() {
		return failed;
	}

	IplImage* nextFrame() {
		if (!picked) {
			picked = true;
			return image;
		} else return NULL;
	}

	void close() {
		cvReleaseImage(&image);
		image = NULL;
	}

	char* getFilename() {
		return filename;
	}

private:
	IplImage* image;
	char filename[200];
	bool picked;

	bool failed;
};

} } }