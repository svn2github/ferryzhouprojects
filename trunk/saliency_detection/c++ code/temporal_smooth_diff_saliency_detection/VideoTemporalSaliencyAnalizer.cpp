#include "VideoTemporalSaliencyAnalizer.h"

#include "VideoTemporalSaliencyAnalizerImpl.h"

VideoTemporalSaliencyAnalizer::VideoTemporalSaliencyAnalizer(void) {
	impl = new VideoTemporalSaliencyAnalizerImpl();
}

VideoTemporalSaliencyAnalizer::~VideoTemporalSaliencyAnalizer(void)
{
}

void VideoTemporalSaliencyAnalizer::setInput(const std::string& inputPathRegex, int startIndex, int endIndex) {
	impl->setInput(inputPathRegex, startIndex, endIndex);
}

void VideoTemporalSaliencyAnalizer::setOutput(const std::string& outputDirectory, int indexStringLength) {
	impl->setOutput(outputDirectory, indexStringLength);
}

void VideoTemporalSaliencyAnalizer::setKernel(Kernel1D* kernel) {
	impl->setKernel(kernel);
}

void VideoTemporalSaliencyAnalizer::setImageSizeScalingFactor(double factor) {
	impl->setImageSizeScalingFactor(factor);
}

void VideoTemporalSaliencyAnalizer::setSaliencyValueScalingFactor(double factor) {
	impl->setSaliencyValueScalingFactor(factor);
}

void VideoTemporalSaliencyAnalizer::setFinalSmooth(bool on) {
	impl->setFinalSmooth(on);
}


void VideoTemporalSaliencyAnalizer::run() {
	impl->run();
}
