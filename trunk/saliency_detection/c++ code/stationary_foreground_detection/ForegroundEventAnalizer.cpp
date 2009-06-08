#include "ForegroundEventAnalizer.h"

#include <fvision/utils.h>
#include <cassert>

using namespace std;

ForegroundEventAnalizer::ForegroundEventAnalizer(void) {
	width = -1;
	height = -1;
}

ForegroundEventAnalizer::~ForegroundEventAnalizer(void) {
	fvision::deepDelete(pixelEventManagers);
}

void ForegroundEventAnalizer::setStartIndex(int index) {
	assert(index >= 0);
	this->startIndex = index;
}

void ForegroundEventAnalizer::setALLEventsOutputStream(ostream* os) {
	assert(os != NULL);
	this->allEventsOutputStream = os;
}

void ForegroundEventAnalizer::setSalientEventsOutputStream(ostream* os) {
	assert(os != NULL);
	this->salientEventsOutputStream = os;
}

void ForegroundEventAnalizer::addFrame(const IplImage* binaryImage) {
	assert (binaryImage != NULL && binaryImage->nChannels == 1);
	if (width > 0) assert (binaryImage->width == width && binaryImage->height == height);

	if (pixelEventManagers.size() == 0) {
		this->width = binaryImage->width;
		this->height = binaryImage->height;
		initializePixelEventManagers();
	}

	processBinaryImage(binaryImage);	
}

void ForegroundEventAnalizer::initializePixelEventManagers() {
	pixelEventManagers.resize(width * height, NULL);
	int index = 0;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			pixelEventManagers[index++] = new PixelEventManager(j, i);
		}
	}
}

void ForegroundEventAnalizer::processBinaryImage(const IplImage* binaryImage) {
	int index = 0;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			PixelEventManager* manager = pixelEventManagers[index++];
			manager->input(binaryImage->imageData[j * binaryImage->widthStep + i]);
			if (manager->isLastEventJustFinished()) {
				PixelEvent event = manager->getLastEvent();
				if (startIndex != 0) {
					event.shiftIndex(startIndex);
				}
				(*allEventsOutputStream)<<event<<endl;
				if (event.isSalient()) {
					(*salientEventsOutputStream)<<event<<endl;
				}
			}
		}
	}	
}

