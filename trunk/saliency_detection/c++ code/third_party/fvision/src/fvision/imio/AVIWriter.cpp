#include ".\aviwriter.h"

#include <iostream>

using namespace std;

namespace fvision {

AVIWriter::AVIWriter() {
	pvw = NULL;
}

AVIWriter::~AVIWriter(void) {
	cvReleaseVideoWriter(&pvw);
}

AVIWriter* AVIWriter::create(const char* path, int width, int height) {		
	CvVideoWriter* pvw = cvCreateVideoWriter(path, -1, 30, cvSize(width, height));
	if (!pvw) {
		cout<<"can't create video writer!"<<endl;
		return NULL;
	} else {
		AVIWriter* writer = new AVIWriter();
		writer->pvw = pvw;
		return writer;
	}
}

void AVIWriter::writeFrame(const IplImage* frame) {
	if (pvw) {
		cvWriteFrame(pvw, frame);
	}
}

}
