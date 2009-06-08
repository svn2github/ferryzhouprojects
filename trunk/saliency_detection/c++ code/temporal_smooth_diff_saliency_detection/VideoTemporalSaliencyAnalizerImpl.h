#pragma once

#include "Kernel1D.h"

#include <string>
#include <deque>

#include <cv.h>

//input a sequence
//output the corresponding saliency sequence
//
//
//set an input path regex, index range and interval, 
//set an output directory
//output intermediate results
//outputDirectory + "raw_saliency/"
//                + "
class VideoTemporalSaliencyAnalizerImpl {
public:
	VideoTemporalSaliencyAnalizerImpl(void);
	~VideoTemporalSaliencyAnalizerImpl(void);

public:
	void setInput(const std::string& inputPathRegex, int startIndex, int endIndex);
	void setOutput(const std::string& outputDirectory, int indexStringLength);

	void setKernel(Kernel1D* kernel);

	void setImageSizeScalingFactor(double factor);
	void setSaliencyValueScalingFactor(double factor);
	void setFinalSmooth(bool on);

	void run();

private:
	void computeFirstSaliencyFrame();
	void loadInitialOriginalFrames();
	void computeInitialTemporalSmoothedFrames();
	void computeInitialDiffTemporalFrames();

	void computeTemoralSmoothedDiffFrame();
	
	void computeNextSaliencyFrame();
	void updateNextOriginalFrame();
	void updateNextTemporalSmoothedFrame();
	void updateNextInitialDiffTemporalFrame();

	bool hasNext();

	void outputSaliencyFrame();

	void initializeByPeekingFirstFrame();

	IplImage* loadOriginalFrame(int index);

	//smooth on [startIndex, startIndex + kernelSize - 1]
	void computeSmoothedImage(IplImage* dst, const std::deque<IplImage*>& input, int startIndex);

	void clean();

private:
	std::string inputPathRegex;
	int startIndex;
	int endIndex;
	std::string outputDirectory;
	int indexStringLength;
	std::string rawSaliencyFramePathRegex;

	Kernel1D* kernel;

	int kernelSize;
	int halfKernelSize;

	double imageSizeScalingFactor;
	double saliencyValueScalingFactor;
	bool finalSmooth;

private:
	int outputIndex;
	int realStartIndex;
	int realEndIndex;

	//data
	std::deque<IplImage*> originalImages; //[-2n, 2n], n = (kernelsize - 1) / 2
	std::deque<IplImage*> temporalSmoothedImages; //[-n, n],  t[i] = {o[i-n], ... o[i + n]}
	std::deque<IplImage*> diffTemporalImages; //[-n, n], d[i] = |t[i] - o[i]|
	IplImage* saliencyImage; //s[i] = {d[i - n], ..., d[i + n]
	IplImage* finalOutputImage;

	CvSize originalImageSize;
	CvSize processingImageSize;

	uchar* temporalPixelBuf;
	IplImage* imageBuffer;

	static const int PATH_SIZE = 500;
	char inputPath[PATH_SIZE];
	char outputPath[PATH_SIZE]; 
};
