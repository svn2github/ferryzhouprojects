#ifndef BACKGROUND_IMAGE_MANAGER_H_
#define BACKGROUND_IMAGE_MANAGER_H_

#include "PixelDifferenceCalculator.h"

#include <cv.h>

class BackgroundImageManager {
public:
	BackgroundImageManager(const IplImage* backgroundImage);
	~BackgroundImageManager(void);

public:
	//return a gray scale image
	virtual void computeForegroundDifference(const IplImage* testImage, IplImage* differenceImage);

	void setUpdatingRatio(double ratio);

	//if the pixel is not salient, update it with new pixel, b(t+1) = (1 - w) * b(t) + w * p(t+1)
	//b is the background pixel and p is the image pixel
	virtual void updateBackground(const IplImage* testImage, const IplImage* binarySaliencyImage);

	const IplImage* getBackgroundImage() const { return backgroundImage; }

protected:
	IplImage* doubleBackgroundImage;
	IplImage* backgroundImage;
	double updatingRatio;

	PixelDifferenceCalculator* differenceCalculator;
};

class MemorableBackgroundImageManager : public BackgroundImageManager {
public:
	MemorableBackgroundImageManager(const IplImage* backgroundImage);
public:
	void computeForegroundDifference(const IplImage* testImage, IplImage* differenceImage);
	void updateBackground(const IplImage* testImage, const IplImage* binarySaliencyImage);
private:
	IplImage* initalBackgroundImage;
	IplImage* differenceImage2;
};	
#endif // BACKGROUND_IMAGE_MANAGER_H_
