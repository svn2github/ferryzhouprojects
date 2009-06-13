#include "PathRegexImageInputHandler.h"

#include <highgui.h>

namespace image_io { namespace impl {

PathRegexImageInputHandler::PathRegexImageInputHandler(const std::string& pathRegex, int startIndex, int endIndex, int interval) {
	this->pathRegex = pathRegex;
	this->startIndex = startIndex;
	this->endIndex = endIndex;
	this->interval = interval;

	curIndex = startIndex;

	pathBuf = new char[pathRegex.size() + 10];

	lastImagePointer = NULL;
}

PathRegexImageInputHandler::~PathRegexImageInputHandler(void) {
	delete[] pathBuf;
	if (lastImagePointer != NULL) {
		cvReleaseImage(&lastImagePointer);
	}
}

IplImage* PathRegexImageInputHandler::read() {
	if (lastImagePointer != NULL) {
		cvReleaseImage(&lastImagePointer);
		lastImagePointer = NULL;
	}
	if (curIndex >= endIndex) return NULL;
	sprintf_s(pathBuf, pathRegex.size() + 10, pathRegex.c_str(), curIndex);
	lastImagePointer = cvLoadImage(pathBuf, 1);
	curIndex += interval;
	return lastImagePointer;
}

}}