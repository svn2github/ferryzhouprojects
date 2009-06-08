#ifndef VIDEO_TEMPORAL_SALIENCY_ANALIZER_H_
#define VIDEO_TEMPORAL_SALIENCY_ANALIZER_H_

#include "Kernel1D.h"

#include <string>

#include <cv.h>

class VideoTemporalSaliencyAnalizerImpl;

class VideoTemporalSaliencyAnalizer {
public:
	VideoTemporalSaliencyAnalizer(void);
	~VideoTemporalSaliencyAnalizer(void);

public:
	void setInput(const std::string& inputPathRegex, int startIndex, int endIndex);
	void setOutput(const std::string& outputDirectory, int indexStringLength);

	void setKernel(Kernel1D* kernel);

	void setImageSizeScalingFactor(double factor);
	void setSaliencyValueScalingFactor(double factor);
	void setFinalSmooth(bool on);

	void run();

private:
	VideoTemporalSaliencyAnalizerImpl* impl;
};

#endif // VIDEO_TEMPORAL_SALIENCY_ANALIZER_H_
