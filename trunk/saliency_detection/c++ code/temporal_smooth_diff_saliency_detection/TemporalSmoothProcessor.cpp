#include "TemporalSmoothProcessor.h"

TemporalSmoothProcessor::TemporalSmoothProcessor(void)
{
}

TemporalSmoothProcessor::~TemporalSmoothProcessor(void)
{
}

IplImage* TemporalSmoothProcessor::queryImage(int index) const {
	if (index < 0 || index >= (topIndex + buffer.size())) return NULL;
	return buffer[index - topIndex];
}

bool TemporalSmoothProcessor::bufferNext() {
	
}

bool TemporalSmoothProcessor::releaseTop() {

}