#pragma once

#include "ImageSequenceReader.h"

#include <iostream>
#include <set>

namespace fvision {

template <typename ImageProcessor>
void processSequence(ImageSequenceReader* imageReader, ImageProcessor* processor) {
	IplImage* frame = NULL;
	int frameId = 0;
	while ((frame = imageReader->nextFrame()) != NULL) {
		processor->process(frameId, frame);
		if (frameId % 10 == 0) std::cout<<"processed "<<frameId<<" frames ......."<<std::endl;
		frameId++;
	}
}

template<typename ImageProcessor>
class SequenceProcessor{
public:
	SequenceProcessor<ImageProcessor>() {
		maxFrameId = -1;
		verbose = true;
		useIdSet = false;
	}

public:
	void setMaxFrameId(size_t maxId) {
		this->maxFrameId = maxId;
	}

	void setVerbose(bool on) {
		this->verbose = on;
	}

	void setFrameIds(const std::set<int>& frameIds) {
		useIdSet = true;
		this->frameIds = frameIds;
	}

public:
	void process(ImageSequenceReader* imageReader, ImageProcessor* processor) {
		IplImage* frame = NULL;
		size_t frameId = 0;
		while (frameId <= maxFrameId && (frame = imageReader->nextFrame()) != NULL) {
			if (verbose && frameId % 10 == 0) std::cout<<"processed "<<frameId<<" frames ......."<<std::endl;
			if (useIdSet && !isFrameIdExists((int)frameId)) {
				frameId++;
				continue;
			}
			processor->process((int)frameId, frame);
			frameId++;
		}
	}

private:
	bool isFrameIdExists(int frameId) {
		return (frameIds.find(frameId) != frameIds.end());
	}
private:
	size_t maxFrameId;
	bool verbose;
	bool useIdSet;
	std::set<int> frameIds;
};
}
