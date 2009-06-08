#include "PhaseSaliencyAnalizer.h"

#include "PhaseSaliencyCalculator3D.h"

#include <cassert>
#include <fvision/utils.h>
#include <iostream>

using namespace std;

PhaseSaliencyAnalizer::PhaseSaliencyAnalizer(void) {
	psc = new GrayScalePhaseSaliencyCalculator3D();
}

PhaseSaliencyAnalizer::~PhaseSaliencyAnalizer(void) {
	releaseSrcs();
	delete psc;
}

void PhaseSaliencyAnalizer::setInput(const std::string& inputPathRegex, int startIndex, int endIndex) {
	this->inputPathRegex = inputPathRegex;
	this->startIndex = startIndex;
	this->endIndex = endIndex;
}

void PhaseSaliencyAnalizer::setOutput(const std::string& outputDirectory, int indexStringLength) {
	this->outputDirectory = outputDirectory;
	this->indexStringLength = indexStringLength;
	string fileRegex = "im_%05d.png";
	fileRegex[5] = '0' + indexStringLength;
	string rawSaliencyFrameDirectory = outputDirectory + "raw_saliency/";
	fvision::JFile jfile;
	if (!jfile.isFileExists(rawSaliencyFrameDirectory.c_str())) {
		jfile.createDirectory(rawSaliencyFrameDirectory.c_str());
	}
	this->rawSaliencyFramePathRegex = rawSaliencyFrameDirectory + fileRegex;
}

void PhaseSaliencyAnalizer::setImageSizeScalingFactor(double factor) {
	this->imageSizeScalingFactor = factor;
}

void PhaseSaliencyAnalizer::setSegmentSize(int size) {
	this->segmentSize = size;
}

void PhaseSaliencyAnalizer::setMotionStep(int step) {
	this->motionStep = step;
}

void PhaseSaliencyAnalizer::setFinalSmoothKernelSize(int size) {
	this->smoothKernelSize = size;
}

void PhaseSaliencyAnalizer::run() {
	initializeByPeekingFirstFrame();

	psc->setMotionStep(motionStep);
	psc->setWidthHeightLength(processingImageSize.width, processingImageSize.height, segmentSize);

	//compute for every <segment_length> frames
	//[startIndex, startIndex + L]
	int segmentNum = (endIndex - startIndex) / segmentSize;
	int segmentStart = 0;
	int segmentEnd = 0;
	for (int i = 0; i < segmentNum; i++) {
		segmentStart = startIndex + i * segmentSize;
		segmentEnd = segmentStart + segmentSize;
		bool hitEnd = process(segmentStart, segmentEnd);
		if (hitEnd) return;
	}
	
	int lastSegmentSize = endIndex - segmentEnd;
	if (lastSegmentSize == 0) return;
	psc->setWidthHeightLength(processingImageSize.width, processingImageSize.height, lastSegmentSize);
	process(segmentEnd, endIndex);
	//
}

bool PhaseSaliencyAnalizer::process(int segmentStart, int segmentEnd) {
	cout<<"processing "<<segmentStart<<" - "<<segmentEnd<<endl;
	loadSrcs(segmentStart, segmentEnd);
	bool hitEnd = false;
	if (srcs.size() != (segmentEnd - segmentStart)) {
		psc->setWidthHeightLength(processingImageSize.width, processingImageSize.height, (int)srcs.size());
		hitEnd = true;
	}
	psc->compute(srcs, dsts);
	saveDsts(segmentStart);
	releaseSrcs();
	return hitEnd;
}

void PhaseSaliencyAnalizer::loadSrcs(int segmentStart, int segmentEnd) {
	srcs.resize(0);
	for (int i = segmentStart; i < segmentEnd; i++) {
		sprintf_s(path, PATH_SIZE, inputPathRegex.c_str(), i);
		IplImage* im = cvLoadImage(path, 0);
		if (im == NULL) {
			return;
		}
		cvResize(im, resizedSrcImageBuffer);
		srcs.push_back(cvCloneImage(resizedSrcImageBuffer));
		cvReleaseImage(&im);
	}
}

void PhaseSaliencyAnalizer::releaseSrcs() {
	for (size_t i = 0; i < srcs.size(); i++) {
		cvReleaseImage(&(srcs[i]));
	}
}

void PhaseSaliencyAnalizer::saveDsts(int segmentStart) {
	for (int i = 0; i < (int)dsts.size(); i++) {
		cvSmooth(dsts[i], dsts[i], CV_GAUSSIAN, smoothKernelSize);
		cvConvertScale(dsts[i], outputImageBuffer, 255);		
		sprintf_s(path, PATH_SIZE, rawSaliencyFramePathRegex.c_str(), i + segmentStart);
		cvSaveImage(path, outputImageBuffer);
	}	
}

void PhaseSaliencyAnalizer::initializeByPeekingFirstFrame() {
	sprintf_s(path, PATH_SIZE, inputPathRegex.c_str(), startIndex);
	IplImage* image = cvLoadImage(path, 0);

	if (image == NULL) throw "can not load start frame!";
	originalImageSize = cvGetSize(image);

	processingImageSize = cvSize(originalImageSize.width / imageSizeScalingFactor, originalImageSize.height / imageSizeScalingFactor);

	resizedSrcImageBuffer = cvCreateImage(processingImageSize, 8, 1);
	outputImageBuffer = cvCreateImage(processingImageSize, 8, 1);
	smoothedImageBuffer = cvCreateImage(processingImageSize, 8, 1);

	cvReleaseImage(&image);
}