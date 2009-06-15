#ifndef FOREGROUND_SALIENCY_ANALIZER_IMPL_H_
#define FOREGROUND_SALIENCY_ANALIZER_IMPL_H_

#include "ImageInputHandler.h"
#include "ImageOutputHandler.h"


//use first N frames for background extraction
//then for each new frame
//   compute difference between new pixel and the background pixel
//now we have a difference image
//we first use a high threshold to filter out significant foreground pixels
//then we compute the saliency probability image based on the previous saliency image and new classified saliency image
//
//two binary image B_H and B_L, based on two thresholds and background knowledge
//prev binary image B_P
//B_1 = B_H | B_P
//B_2 = Dilate(B_1)
//B_3 = B_2 & B_L
//
//finally, update background image based on B_3
//
// [20, 60]
class ForegroundSaliencyAnalizerImpl {
public:
	ForegroundSaliencyAnalizerImpl(void);
	~ForegroundSaliencyAnalizerImpl(void);
public:
	void setImageInputHandler(image_io::ImageInputHandler* imageInputHandler) {
		assert (imageInputHandler != NULL);
		this->imageInputHandler = imageInputHandler;
	}
	void setImageOutputHandler(image_io::ImageOutputHandler* imageOutputHandler) {
		assert (imageOutputHandler != NULL);
		this->imageOutputHandler = imageOutputHandler;
	}

	void run();

	void setTrainingFrameCount(int count) {
		assert (count > 0 && count < 10000);
		this->trainingFrameCount = count;
	}

	void setThresholds(double higherThreshold, double lowerThreshold) {
		assert (higherThreshold > lowerThreshold && lowerThreshold > 0);
		this->higherThreshold = higherThreshold;
		this->lowerThreshold = lowerThreshold;
	}

	void setBackgroundUpdatingRatio(double ratio) {
		this->updatingRatio = ratio;
	}

private:
	image_io::ImageInputHandler* imageInputHandler;
	image_io::ImageOutputHandler* imageOutputHandler;

	int trainingFrameCount;

	double higherThreshold;
	double lowerThreshold;

	double updatingRatio;
	
	IplImage* originalImage;
	IplImage* differenceImage;
	IplImage* binarySaliencyImage;
};

#endif // FOREGROUND_SALIENCY_ANALIZER_IMPL_H_
