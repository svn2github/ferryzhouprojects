#pragma once

#include <iostream>

#include <ferry/util/StringUtil.h>

#include "ImageSequenceReader.h"

using namespace std;
using namespace ferry::util;

namespace ferry { namespace imutil {
	namespace io {

/*!
* read avi video
*/
class AVIImageSequenceReader : public ImageSequenceReader {
public:
	AVIImageSequenceReader(const char* path) : ImageSequenceReader(path) {
		capture = cvCaptureFromAVI(path);
		if (capture == NULL) {
			cout<<"capture initialization failed!"<<endl;
			failed = true;
		} else {
			failed = false;
		}
		curIndex = -1;
		string filenameBuf;
		sprintf(this->filename, "%s", getFileName(path, filenameBuf));
	}

	~AVIImageSequenceReader(void) {
		if (capture != NULL) {
			cvReleaseCapture(&capture);
			capture = NULL;
		}
	}

public:
	bool isFailed() {
		return failed;
	}

	IplImage* nextFrame() {

		if (failed) return NULL;
		//curIndex++;
		//sprintf(filename, "im_%05d", curIndex);
		return cvQueryFrame(capture);
	}

	void close() {
		cvReleaseCapture(&capture);
		capture = NULL;
	}

	char* getFilename() {
		return filename;
	}

private:
	CvCapture* capture;
	//IplImage* frame;
	bool failed;
	int curIndex;
	char filename[200];
};

} } }
