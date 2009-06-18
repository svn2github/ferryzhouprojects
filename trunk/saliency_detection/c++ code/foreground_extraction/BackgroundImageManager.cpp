#include "BackgroundImageManager.h"

#include <highgui.h>
#include <iostream>
using namespace std;

BackgroundImageManager::BackgroundImageManager(const IplImage* backgroundImage) {
	assert (backgroundImage != NULL);
	this->backgroundImage = cvCloneImage(backgroundImage);
	this->doubleBackgroundImage = cvCreateImage(cvGetSize(backgroundImage), IPL_DEPTH_64F, backgroundImage->nChannels);
	cvConvert(backgroundImage, doubleBackgroundImage);

	differenceCalculator = new DistancePixelDifferenceCalculator();
	//cvNamedWindow("background");
}

BackgroundImageManager::~BackgroundImageManager(void) {
	cvReleaseImage(&backgroundImage);
}

//simply take difference between background and testimage
void BackgroundImageManager::computeForegroundDifference(const IplImage* testImage, IplImage* differenceImage) {
	computeImageDifference(backgroundImage, testImage, differenceImage, differenceCalculator);
	//cvShowImage("background", backgroundImage);
}

void BackgroundImageManager::setUpdatingRatio(double ratio) {
	assert(ratio >= 0 && ratio <= 1);
	this->updatingRatio = ratio;
}

void BackgroundImageManager::updateBackground(const IplImage* testImage, const IplImage* binarySaliencyImage) {
	double* background = (double*)doubleBackgroundImage->imageData;
	const uchar* newpixels = (const uchar*)testImage->imageData;
	const uchar* saliency = (const uchar*)binarySaliencyImage->imageData;

	double alpha = 1 - updatingRatio;
	int index = 0;
	int offset = 0;
	for (int i = 0; i < doubleBackgroundImage->height; i++) {
		offset = doubleBackgroundImage->widthStep * i / 8;
		for (int j = 0; j < doubleBackgroundImage->width; j++) {
			index = offset + j * doubleBackgroundImage->nChannels;
			uchar s = saliency[i * binarySaliencyImage->widthStep + j];
			if (s == 0) {
				for (int k = 0; k < 3; k++) {
					background[index + k] = background[index + k] * alpha + newpixels[index + k] * updatingRatio;
				}
			}
		}
	}
	cvConvert(doubleBackgroundImage, backgroundImage);
}

MemorableBackgroundImageManager::MemorableBackgroundImageManager(const IplImage* backgroundImage)
: BackgroundImageManager (backgroundImage) 
{
	initalBackgroundImage = cvCloneImage(backgroundImage);
	differenceImage2 = cvCreateImage(cvGetSize(backgroundImage), 8, 1);
}

void MemorableBackgroundImageManager::computeForegroundDifference(const IplImage* testImage, IplImage* differenceImage) {
	computeImageDifference(backgroundImage, testImage, differenceImage, differenceCalculator);
	computeImageDifference(initalBackgroundImage, testImage, differenceImage2, differenceCalculator);
	cvMin(differenceImage, differenceImage2, differenceImage);
	//cvShowImage("background", backgroundImage);
}

void MemorableBackgroundImageManager::updateBackground(const IplImage* testImage, const IplImage* binarySaliencyImage) {
	BackgroundImageManager::updateBackground(testImage, binarySaliencyImage);
}