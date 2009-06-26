#include "ForegroundSaliencyClassifier.h"


AdaptiveForegroundSaliencyClassifier::AdaptiveForegroundSaliencyClassifier(double higherThreshold, double lowerThreshold) {
	this->higherThreshold = higherThreshold;
	this->lowerThreshold = lowerThreshold;
	this->lastBinarySaliencyImage = NULL;
	this->lowerThresholdBinarySaliencyImage = NULL;
}

void AdaptiveForegroundSaliencyClassifier::classify(const IplImage* differenceImage, IplImage* binarySaliencyImage) {
	cvCmpS(differenceImage, higherThreshold, binarySaliencyImage, CV_CMP_GT);
	if (lastBinarySaliencyImage != NULL) {
		cvOr(lastBinarySaliencyImage, binarySaliencyImage, binarySaliencyImage);
	} else {
		lastBinarySaliencyImage = cvCloneImage(binarySaliencyImage);
		lowerThresholdBinarySaliencyImage = cvCloneImage(binarySaliencyImage);
	}
	cvDilate(binarySaliencyImage, binarySaliencyImage, NULL, 2);
	cvCmpS(differenceImage, lowerThreshold, lowerThresholdBinarySaliencyImage, CV_CMP_GT);
	cvAnd(binarySaliencyImage, lowerThresholdBinarySaliencyImage, binarySaliencyImage);
	cvCopy(binarySaliencyImage, lastBinarySaliencyImage);
}