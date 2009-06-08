#pragma once

#include "PhaseSaliencyCalculator3D.h"

#include <string>

#include <cv.h>

class PhaseSaliencyAnalizer {
public:
	PhaseSaliencyAnalizer(void);
	~PhaseSaliencyAnalizer(void);

public:
	void setInput(const std::string& inputPathRegex, int startIndex, int endIndex);
	void setOutput(const std::string& outputDirectory, int indexStringLength);

	void setImageSizeScalingFactor(double factor);
	void setSegmentSize(int size);
	void setMotionStep(int step);
	void setFinalSmoothKernelSize(int size);

	void run();

private:
	void initializeByPeekingFirstFrame();

	//if hit end, return true
	bool process(int segmentStart, int segmentEnd);

	//return num of loaded images
	void loadSrcs(int segmentStart, int segmentEnd);
	void releaseSrcs();
	void saveDsts(int segmentStart);

private:
	std::string inputPathRegex;
	int startIndex;
	int endIndex;
	std::string outputDirectory;
	int indexStringLength;
	std::string rawSaliencyFramePathRegex;

	double imageSizeScalingFactor;
	int segmentSize;
	int motionStep;
	int smoothKernelSize;

private:
	static const int PATH_SIZE = 300;
	char path[PATH_SIZE];

	CvSize originalImageSize;
	CvSize processingImageSize;

	IplImage* resizedSrcImageBuffer;
	IplImage* smoothedImageBuffer;
	IplImage* outputImageBuffer;

	std::vector<IplImage*> srcs;
	std::vector<IplImage*> dsts;

	PhaseSaliencyCalculator3D* psc;
};
