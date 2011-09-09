#pragma once

#include <iostream>

#include <cv.h>
#include <highgui.h>

using namespace std;

namespace ferry { namespace imutil {	namespace io {

class AVIWriter
{
public:
	AVIWriter(const char* path, int width, int height) {
		//this->path = path;

		pvw = cvCreateVideoWriter(path, -1, 30, cvSize(width, height));

		if (!pvw) {
			cout<<"can't create video writer!"<<endl;
			return;
		}
	}

	~AVIWriter(void) {
		cvReleaseVideoWriter(&pvw);
	}

public:
	void writeFrame(IplImage* frame) {
		if (pvw) {
			cvWriteFrame(pvw, frame);
		}
	}

private:
	//const char* path;
	CvVideoWriter* pvw;
};

} } }