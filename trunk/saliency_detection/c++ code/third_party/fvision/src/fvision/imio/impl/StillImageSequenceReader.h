#ifndef FVISION_IMIO_STILL_IMAGE_READER_H_
#define FVISION_IMIO_STILL_IMAGE_READER_H_

#include "../ImageSequenceReader.h"

namespace fvision {

/**
 * read single still image
 */
class StillImageSequenceReader : public ImageSequenceReader {
public:
	StillImageSequenceReader(const char* path);
	~StillImageSequenceReader(void);

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

	const char* getFilename() const {
		return filename;
	}

private:
	IplImage* image;
	char filename[200];
	bool picked;

	bool failed;
};

}
#endif // FVISION_IMIO_STILL_IMAGE_READER_H_