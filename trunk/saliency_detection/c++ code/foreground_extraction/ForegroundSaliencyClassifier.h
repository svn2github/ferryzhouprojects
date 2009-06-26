#ifndef FOREGROUND_SALIENCY_CLASSIFIER_H_
#define FOREGROUND_SALIENCY_CLASSIFIER_H_

#include <cv.h>

class ForegroundSaliencyClassifier {
public:
	virtual ~ForegroundSaliencyClassifier(void) {}
	virtual void classify(const IplImage* differenceImage, IplImage* binarySaliencyImage) = 0;
};

class SimpleForegroundSaliencyClassifier : public ForegroundSaliencyClassifier {
public:
	SimpleForegroundSaliencyClassifier(double threshold) {
		assert (threshold > 0);
		this->threshold = threshold;
	}
public:
	void classify(const IplImage* differenceImage, IplImage* binarySaliencyImage) {
		cvCmpS(differenceImage, threshold, binarySaliencyImage, CV_CMP_GT);
	}

private:
	double threshold;
};

//we first use a high threshold to filter out significant foreground pixels
//then we compute the saliency probability image based on the previous saliency image and new classified saliency image
//
//two binary image B_H and B_L, based on two thresholds and background knowledge
//prev binary image B_P
//B_1 = B_H | B_P
//B_2 = Dilate(B_1)
//B_3 = B_2 & B_L
class AdaptiveForegroundSaliencyClassifier : public ForegroundSaliencyClassifier {
public:
	AdaptiveForegroundSaliencyClassifier(double higherThreshold, double lowerThreshold);

public:
	void classify(const IplImage* differenceImage, IplImage* binarySaliencyImage);

private:
	double higherThreshold;
	double lowerThreshold;

	IplImage* lastBinarySaliencyImage;
	IplImage* lowerThresholdBinarySaliencyImage;
};

#endif // FOREGROUND_SALIENCY_CLASSIFIER_H_
