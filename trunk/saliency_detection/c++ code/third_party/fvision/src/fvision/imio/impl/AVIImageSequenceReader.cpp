#include ".\aviimagesequencereader.h"

#include <fvision/utils/string_utils.h>
#include <iostream>

using namespace std;

namespace fvision {

AVIImageSequenceReader::AVIImageSequenceReader(const char* path) : ImageSequenceReader(path) {
	capture = cvCaptureFromAVI(path);
	if (capture == NULL) {
		cout<<"capture initialization failed!"<<endl;
		failed = true;
	} else {
		failed = false;
	}
	curIndex = -1;
	string filenameBuf;
	sprintf_s(this->filename, 200, "%s", getFileName(path, filenameBuf));
}

AVIImageSequenceReader::~AVIImageSequenceReader(void) {
	if (capture != NULL) {
		cvReleaseCapture(&capture);
		capture = NULL;
	}
}


}