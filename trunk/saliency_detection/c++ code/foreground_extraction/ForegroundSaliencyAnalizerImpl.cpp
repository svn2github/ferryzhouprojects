#include "ForegroundSaliencyAnalizerImpl.h"

#include "BackgroundImageManager.h"
#include "InitialBackgroundAnalizerClusterImpl.h"
#include "ForegroundSaliencyClassifier.h"

#include <image_io.h>

#include <iostream>
#include <vector>

using namespace std;

ForegroundSaliencyAnalizerImpl::ForegroundSaliencyAnalizerImpl(void) {

	imageInputHandler = NULL;
	imageOutputHandler = NULL;

	trainingFrameCount = 100;
	higherThreshold = 60;
	lowerThreshold = 40;
	updatingRatio = 0.05;

	originalImage = NULL;
	differenceImage = NULL;
	binarySaliencyImage = NULL;

}

ForegroundSaliencyAnalizerImpl::~ForegroundSaliencyAnalizerImpl(void) {
	delete imageInputHandler;
	delete imageOutputHandler;
}

void ForegroundSaliencyAnalizerImpl::run() {
	//for first set of frames, use training to get background image
	InitialBackgroundAnalizer* backgroundAnalizer = new InitialBackgroundAnalizerClusterImpl();
	std::vector<IplImage*> initialTrainingFrames;
	for (int i = 0; i < trainingFrameCount; i++) {
		IplImage* frame = imageInputHandler->read();
		if (frame != NULL) {
			backgroundAnalizer->addTrainingFrame(frame);
			initialTrainingFrames.push_back(cvCloneImage(frame));
		}
	}
	if (initialTrainingFrames.size() == 0) {
		cout<<"no image input!"<<endl;
		return;
	}
	const IplImage* backgroundImage = backgroundAnalizer->getBackgroundImage();
	//BackgroundImageManager* backgroundImageManager = new BackgroundImageManager(backgroundImage);
	BackgroundImageManager* backgroundImageManager = new MemorableBackgroundImageManager(backgroundImage);
	backgroundImageManager->setUpdatingRatio(updatingRatio);
	delete backgroundAnalizer;

	ForegroundSaliencyClassifier* classifier = new AdaptiveForegroundSaliencyClassifier(higherThreshold, lowerThreshold);
	//ForegroundSaliencyClassifier* classifier = new SimpleForegroundSaliencyClassifier((higherThreshold + lowerThreshold) / 2);

	CvSize imageSize = cvGetSize(initialTrainingFrames[0]);
	originalImage = NULL;
	differenceImage = cvCreateImage(imageSize, 8, 1);
	binarySaliencyImage = cvCreateImage(imageSize, 8, 1);

	//classify initial frames
	for (size_t i = 0; i < initialTrainingFrames.size(); i++) {
		originalImage = initialTrainingFrames[i];
		backgroundImageManager->computeForegroundDifference(originalImage, differenceImage);
		classifier->classify(differenceImage, binarySaliencyImage);
		imageOutputHandler->write(binarySaliencyImage);
		cvReleaseImage(&originalImage);
	}

	//classify rest images
	//image_io::ImageOutputHandler* originalImageOutputHandler = image_io::ImageOutputHandlerFactory().createWindowImageOutputHandler("original", 1);
	while ((originalImage = imageInputHandler->read()) != NULL) {
		//originalImageOutputHandler->write(originalImage);
		backgroundImageManager->computeForegroundDifference(originalImage, differenceImage);
		classifier->classify(differenceImage, binarySaliencyImage);
		backgroundImageManager->updateBackground(originalImage, binarySaliencyImage);
		imageOutputHandler->write(binarySaliencyImage);
	}

	cvReleaseImage(&differenceImage);
	cvReleaseImage(&binarySaliencyImage);
	differenceImage = NULL;
	binarySaliencyImage = NULL;

	delete backgroundImageManager;
	delete classifier;
}
