#include "ImageSequenceBufferImpl.h"

#include <cassert>

namespace fvision {

ImageSequenceBufferImpl::ImageSequenceBufferImpl(UnifiedImageSequenceReader* uisr, int leftSpan, int rightSpan) {

	assert(uisr != NULL);
	assert(leftSpan  >= 0  && leftSpan  < 100);
	assert(rightSpan >= 0  && rightSpan < 100);

	this->uisr        = uisr;
	this->leftSpan    = leftSpan;
	this->rightSpan   = rightSpan;
	this->bufferSize  = leftSpan + rightSpan + 1;

	this->curIndex = -1;

	arriveEnd = false;

	minNonNullIndex = 0;
	maxNonNullIndex = 0;
}

ImageSequenceBufferImpl::~ImageSequenceBufferImpl() {
	delete uisr;
	if (maxNonNullIndex != -1) {//read something
		for (int i = 0; i < bufferSize; i++) {
			cvReleaseImage(&(buffer[i]));
		}
	}
}

void ImageSequenceBufferImpl::init() {

	IplImage* frame = uisr->nextFrame();
	if (frame == NULL) { //can read nothing, do not initialize buffer and 
		minNonNullIndex = -1;
		maxNonNullIndex = -1;
		arriveEnd       = true;
	} else {
		for (int i = 0; i < bufferSize; i++) {
			buffer.push_back(cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels));
		}
		cvCopy(frame, buffer[leftSpan]);
		int i = leftSpan + 1;
		for ( ; i < bufferSize; i++) {
			frame = uisr->nextFrame();
			if (frame != NULL) {
				cvCopy(frame, buffer[i]);
			} else {
				arriveEnd = true;
				break;
			}
		}

		curIndex = 0;

		minNonNullIndex = curIndex - leftSpan;
		if (minNonNullIndex < 0) minNonNullIndex  = 0;
		maxNonNullIndex = i - 1 - leftSpan;

		bufferIndex = leftSpan;
	}

}

IplImage* ImageSequenceBufferImpl::curFrame() const {
	if (isIndexFrameNull(curIndex)) return NULL;
	else return buffer[bufferIndex];
}

bool ImageSequenceBufferImpl::isIndexFrameNull(int index) const {
	return index < minNonNullIndex || index > maxNonNullIndex;
}

IplImage* ImageSequenceBufferImpl::nextFrame() {

	if (curIndex < 0) { //init

		init();

	} else {

		if (arriveEnd) { // do not read more
		} else {
			IplImage* frame = uisr->nextFrame();

			if (frame == NULL) { //reach the end
				arriveEnd        = true;
			} else {
				//find the index to replace
				int replaceIndex = bufferIndex - leftSpan;
				if (replaceIndex < 0) replaceIndex += bufferSize;

				cvCopy(frame, buffer[replaceIndex]);

				maxNonNullIndex++;
			}
		}

		curIndex++;

		minNonNullIndex = curIndex - leftSpan;
		if (minNonNullIndex < 0) minNonNullIndex  = 0;

		bufferIndex++;
		if (bufferIndex == bufferSize) bufferIndex = 0;
	}
	return curFrame();

}

IplImage* ImageSequenceBufferImpl::getFrame(int index) const {

	if (isIndexFrameNull(index)) return NULL;

	int offset = index - curIndex;
	int bIndex = (bufferIndex + offset) % bufferSize;
	if (bIndex < 0) bIndex += bufferSize;
	
	return buffer[bIndex];
}

IplImage* ImageSequenceBufferImpl::curFrameWithOffset(int offset) const {
	assert(offset >= -leftSpan && offset <= rightSpan);

	int bIndex = (bufferIndex + offset) % bufferSize;
	
	return buffer[bIndex];
}



}