#pragma once

#include "IndexedImageBufferProcessor.h"

#include "Kernel1D.h"
#include <vector>

class TemporalSmoothProcessor : public IndexedImageBufferProcessor {
public:
	TemporalSmoothProcessor(void);
	~TemporalSmoothProcessor(void);

public:
	IplImage* queryImage(int index) const;
	bool bufferNext();

protected:
	IndexedImageBufferProcessor* originalImageBuffer;

private:
	int topIndex;
	int innerBeginIndex;
	std::vector<IplImage*> circularBuffer;

	int nextIndex;

	Kernel1D* smoothKernel;
};
