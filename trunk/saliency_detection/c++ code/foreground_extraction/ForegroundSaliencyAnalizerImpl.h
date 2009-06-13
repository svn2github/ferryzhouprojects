#ifndef FOREGROUND_SALIENCY_ANALIZER_IMPL_H_
#define FOREGROUND_SALIENCY_ANALIZER_IMPL_H_

#include "ImageInputHandler.h"
#include "ImageOutputHandler.h"

class ForegroundSaliencyAnalizerImpl {
public:
	ForegroundSaliencyAnalizerImpl(void);
	~ForegroundSaliencyAnalizerImpl(void);
public:
	void setImageInputHandler(ImageInputHandler* imageInputHandler);
	void setImageOutputHandler(ImageOutputHandler* imageOutputHandler);

	void run();



private:
	ImageInputHandler* imageInputHandler;
	ImageOutputHandler* ImageOutputHandler;

	IplImage* backgroundImage;
	IplImage* backgroundImage2; //

	IplImage* saliencyProbability; //if a pixel is near a saliency pixel, then we should use a smaller threshold
};

#endif // FOREGROUND_SALIENCY_ANALIZER_IMPL_H_
