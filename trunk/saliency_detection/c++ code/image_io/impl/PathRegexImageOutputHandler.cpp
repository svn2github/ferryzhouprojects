#include "PathRegexImageOutputHandler.h"

#include <highgui.h>
#include <iostream>

using namespace image_io;
using namespace std;

namespace image_io { namespace impl {

PathRegexImageOutputHandler::PathRegexImageOutputHandler(const std::string& pathRegex, int startIndex, int interval /*= 1*/) {
	this->pathRegex = pathRegex;
	this->startIndex = startIndex;
	this->interval = interval;

	curIndex = startIndex;

	pathBuf = new char[pathRegex.size() + 10];
}

PathRegexImageOutputHandler::~PathRegexImageOutputHandler(void) {
	delete[] pathBuf;
}

void PathRegexImageOutputHandler::write(const IplImage* im) throw(ImageIoException) {
	cvSetErrMode(CV_ErrModeParent);

	sprintf_s(pathBuf, pathRegex.size() + 10, pathRegex.c_str(), curIndex);
	cvSaveImage(pathBuf, im);

	curIndex += interval;

	if (curIndex % 10 == 0) cout<<"output "<<curIndex<<"..........."<<endl;

	if (cvGetErrStatus() < 0) {
		cout<<"error! "<<pathBuf<<endl;
		throw new ImageIoException(pathBuf);
	}
}

}}