#pragma once

#include "../ImageSequenceReader.h"

namespace fvision {
/**
 *	no real images input, just create a new image
 *	filename = 0000, 0001, ... 0011, ... 
 */
class BlankImageSequenceReader : public ImageSequenceReader {
public:
	BlankImageSequenceReader(const CvSize& size, int depth, int channels, CvScalar color);
	~BlankImageSequenceReader(void);

public:
	bool isFailed() {
		return false;
	}

	IplImage* nextFrame();

	void close() {
		cvReleaseImage(&image);
		image = NULL;
	}

	const char* getFilename() const {
		return filename;
	}

private:
	IplImage* image;
	int index;
	char filename[300];
	CvScalar color;
};
}