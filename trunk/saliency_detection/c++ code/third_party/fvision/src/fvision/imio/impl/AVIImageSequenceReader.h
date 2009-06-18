#pragma once

#include "../ImageSequenceReader.h"

#include <highgui.h>

namespace fvision {

/**
 * read avi video
 */
class AVIImageSequenceReader : public ImageSequenceReader {
public:
	AVIImageSequenceReader(const char* path);
	~AVIImageSequenceReader(void);

public:
	bool isFailed() {
		return failed;
	}

	IplImage* nextFrame() {
		if (failed) return NULL;
		return cvQueryFrame(capture);
	}

	void close() {
		cvReleaseCapture(&capture);
		capture = NULL;
	}

	const char* getFilename() const {
		return filename;
	}

private:
	CvCapture* capture;
	//IplImage* frame;
	bool failed;
	int curIndex;
	char filename[200];
};

}
