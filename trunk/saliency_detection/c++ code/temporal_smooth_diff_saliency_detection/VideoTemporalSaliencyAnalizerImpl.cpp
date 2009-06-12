#include "VideoTemporalSaliencyAnalizerImpl.h"

#include <cassert>
#include <fvision/utils.h>

using namespace std;

VideoTemporalSaliencyAnalizerImpl::VideoTemporalSaliencyAnalizerImpl(void) {
	kernel = NULL;

	outputIndex = 0;
	realStartIndex = -1;
	realEndIndex = -1;

	imageSizeScalingFactor = 8;
	saliencyValueScalingFactor = 20; //means no scaling
	finalSmooth = true;
}

VideoTemporalSaliencyAnalizerImpl::~VideoTemporalSaliencyAnalizerImpl(void) {
	if(kernel != NULL) delete kernel;
}

void VideoTemporalSaliencyAnalizerImpl::setKernel(Kernel1D* kernel) {
	assert(kernel != NULL);
	if (this->kernel != NULL) {
		delete[] temporalPixelBuf;
		delete this->kernel;
	}
	this->kernel = kernel;
	kernelSize = (int)kernel->size();
	halfKernelSize = (kernelSize - 1) / 2;
	temporalPixelBuf = new uchar[kernelSize];
}

void VideoTemporalSaliencyAnalizerImpl::run() {
	assert(kernel != NULL);
	computeFirstSaliencyFrame();
	outputSaliencyFrame();
	while (hasNext()) {
		computeNextSaliencyFrame();
		outputSaliencyFrame();
		if (outputIndex % 10 == 0) cout<<"processing "<<outputIndex<<" ........."<<endl;
	}
}

void VideoTemporalSaliencyAnalizerImpl::computeFirstSaliencyFrame() {
	outputIndex = startIndex;
	initializeByPeekingFirstFrame();
	loadInitialOriginalFrames();
	computeInitialTemporalSmoothedFrames();
	computeInitialDiffTemporalFrames();
	computeTemoralSmoothedDiffFrame();
}

void VideoTemporalSaliencyAnalizerImpl::loadInitialOriginalFrames() {
	//start from startIndex
	IplImage* image = loadOriginalFrame(startIndex);
	if (image == NULL) throw "can not load start frame!";

	originalImages.push_back(image);

	//go backward and forward
	for (int i = 1; i <= (int)kernel->size() - 1; i++) {
		image = loadOriginalFrame(startIndex - i);
		if (image == NULL) {
			if (realStartIndex < 0) {
				realStartIndex = startIndex - i + 1;
			}
			image = loadOriginalFrame(realStartIndex);
			assert(image != NULL);
		}
		originalImages.push_front(image);

		image = loadOriginalFrame(startIndex + i);
		if (image == NULL) {
			if (realEndIndex < 0) {
				realEndIndex = startIndex + i - 1;
			}
			image = loadOriginalFrame(realEndIndex);
			assert(image != NULL);
		}
		originalImages.push_back(image);
	}
}

IplImage* VideoTemporalSaliencyAnalizerImpl::loadOriginalFrame(int index) {
	sprintf_s(inputPath, PATH_SIZE, inputPathRegex.c_str(), index);
	IplImage* originalImage = cvLoadImage(inputPath, 0);
	if (originalImage == NULL) return NULL;
	IplImage* resizedImage = cvCreateImage(processingImageSize, 8, 1);
	cvResize(originalImage, resizedImage);
	cvSmooth(resizedImage, resizedImage);
	cvReleaseImage(&originalImage);
	return resizedImage;
}

void VideoTemporalSaliencyAnalizerImpl::computeInitialTemporalSmoothedFrames() {
	for (size_t i = 0; i < kernel->size(); i++) {
		computeSmoothedImage(imageBuffer, originalImages, (int)i);
		temporalSmoothedImages.push_back(cvCloneImage(imageBuffer));
	}
}

void VideoTemporalSaliencyAnalizerImpl::computeInitialDiffTemporalFrames() {
	for (size_t i = 0; i < kernel->size(); i++) {
		cvAbsDiff(originalImages[i + halfKernelSize], temporalSmoothedImages[i], imageBuffer);
		diffTemporalImages.push_back(cvCloneImage(imageBuffer));
	}
}

void VideoTemporalSaliencyAnalizerImpl::computeTemoralSmoothedDiffFrame() {
	computeSmoothedImage(saliencyImage, diffTemporalImages, 0);
}

void VideoTemporalSaliencyAnalizerImpl::computeNextSaliencyFrame() {
	outputIndex++;
	updateNextOriginalFrame();
	updateNextTemporalSmoothedFrame();
	updateNextInitialDiffTemporalFrame();
	computeTemoralSmoothedDiffFrame();
}

void VideoTemporalSaliencyAnalizerImpl::updateNextOriginalFrame() {
	IplImage* image = loadOriginalFrame(outputIndex + kernel->size() - 1);
	if (image == NULL) {
		if (realEndIndex < 0) {
			realEndIndex = outputIndex + kernel->size() - 2;
		}
		image = loadOriginalFrame(realEndIndex);
		assert (image != NULL);
	}
	originalImages.push_back(image);
	image = originalImages.front();
	cvReleaseImage(&image);
	originalImages.pop_front();
}

void VideoTemporalSaliencyAnalizerImpl::updateNextTemporalSmoothedFrame() {
	IplImage* image = temporalSmoothedImages.front();
	computeSmoothedImage(image, originalImages, kernelSize - 1);
	temporalSmoothedImages.push_back(image);
	temporalSmoothedImages.pop_front();
}

void VideoTemporalSaliencyAnalizerImpl::updateNextInitialDiffTemporalFrame() {
	cvAbsDiff(originalImages[kernelSize + halfKernelSize - 1], temporalSmoothedImages.back(), imageBuffer);
	diffTemporalImages.push_back(cvCloneImage(imageBuffer));
	IplImage* image = diffTemporalImages.front();
	cvReleaseImage(&image);
	diffTemporalImages.pop_front();
}

void VideoTemporalSaliencyAnalizerImpl::computeSmoothedImage(IplImage* dst, const std::deque<IplImage*>& input, int startIndex) {
	uchar* dstData = (uchar*)(dst->imageData);
	vector<const uchar*> srcData(kernelSize);
	for (int k = 0; k < kernelSize; k++) {
		srcData[k] = (const uchar*)(input[startIndex + k]->imageData);
	}
	int rowOffset = 0;
	int offset = 0;
	for (int y = 0; y < dst->height; y++) {
		rowOffset = y * input[0]->widthStep;
		for (int x = 0; x < dst->width; x++) {
			offset = rowOffset + x;
			for (int k = 0; k < kernelSize; k++) {
				temporalPixelBuf[k] = srcData[k][offset];
			}
			dstData[offset] = kernel->filter(temporalPixelBuf);
		}
	}
}

bool VideoTemporalSaliencyAnalizerImpl::hasNext() {
	if (realEndIndex < 0) return (outputIndex <= endIndex);
	else return (outputIndex <= endIndex) && (outputIndex <= realEndIndex);
}

void VideoTemporalSaliencyAnalizerImpl::outputSaliencyFrame() {
	sprintf_s(outputPath, PATH_SIZE, rawSaliencyFramePathRegex.c_str(), outputIndex);
	if (saliencyValueScalingFactor > 0) {
		cvScale(saliencyImage, saliencyImage, saliencyValueScalingFactor);
	}
	if (finalSmooth) {
		cvSmooth(saliencyImage, finalOutputImage);
		cvSaveImage(outputPath, finalOutputImage);
	} else {
		cvSaveImage(outputPath, saliencyImage);	
	}
}

void VideoTemporalSaliencyAnalizerImpl::setInput(const std::string& inputPathRegex, int startIndex, int endIndex) {
	this->inputPathRegex = inputPathRegex;
	this->startIndex = startIndex;
	this->endIndex = endIndex;
}

void VideoTemporalSaliencyAnalizerImpl::setOutput(const std::string& outputDirectory, int indexStringLength) {
	assert (indexStringLength > 0 && indexStringLength < 10);
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

void VideoTemporalSaliencyAnalizerImpl::clean() {
	
}

void VideoTemporalSaliencyAnalizerImpl::initializeByPeekingFirstFrame() {
	sprintf_s(inputPath, PATH_SIZE, inputPathRegex.c_str(), startIndex);
	IplImage* image = cvLoadImage(inputPath, 0);

	if (image == NULL) throw "can not load start frame!";
	originalImageSize = cvGetSize(image);

	processingImageSize = cvSize(originalImageSize.width / imageSizeScalingFactor, originalImageSize.height / imageSizeScalingFactor);

	imageBuffer = cvCreateImage(processingImageSize, 8, 1);
	saliencyImage = cvCreateImage(processingImageSize, 8, 1);
	finalOutputImage = cvCreateImage(processingImageSize, 8, 1);

	cvReleaseImage(&image);
}

void VideoTemporalSaliencyAnalizerImpl::setImageSizeScalingFactor(double factor) {
	assert (factor > 0);
	this->imageSizeScalingFactor = factor;
}

void VideoTemporalSaliencyAnalizerImpl::setSaliencyValueScalingFactor(double factor) {
	assert (factor > 0);
	this->saliencyValueScalingFactor = factor;
}

void VideoTemporalSaliencyAnalizerImpl::setFinalSmooth(bool on) {
	this->finalSmooth = on;
}