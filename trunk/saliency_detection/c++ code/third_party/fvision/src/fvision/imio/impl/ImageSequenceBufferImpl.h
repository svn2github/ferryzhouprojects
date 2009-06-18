#ifndef FVISION_IMIO_IMAGE_SEQUENCE_BUFFER_IMPL_H_
#define FVISION_IMIO_IMAGE_SEQUENCE_BUFFER_IMPL_H_

#include "UnifiedImageSequenceReader.h"

#include "../ImageSequenceBuffer.h"

#include <vector>

namespace fvision {

/** Keep a certian images buffer while sequentially reading a image sequence
 * the buffer window is [curIndex - leftSpan, curIndex + rightSpan], images in this window can be retrieved
 * 
 */
class ImageSequenceBufferImpl : public ImageSequenceBuffer {

public:
	/** 
	 * responsible for deleting uisr
	 * assert leftSpan and rightSpan is between [0, 100]
	 */
	ImageSequenceBufferImpl(UnifiedImageSequenceReader* uisr, int leftSpan = 0, int rightSpan = 10);

	~ImageSequenceBufferImpl();

private:
	//non copyable
	ImageSequenceBufferImpl(const ImageSequenceBufferImpl& rhs) {} 
	ImageSequenceBufferImpl& operator= (const ImageSequenceBufferImpl& rhs) {} 

public:

	/** read next frame
	 * return NULL if end
	 * increase curIndex
	 */
	IplImage* nextFrame();

	int getCurIndex() const {
		return curIndex;
	}

	IplImage* curFrame() const;

	///return NULL if the frame is not available or out of buffer window
	IplImage* getFrame(int index) const;

	/** assert offset >= -leftSpan && offset <= rightSpan
	 */
	IplImage* curFrameWithOffset(int offset) const;

private:
	/** Initialize the buffer by reading a set of (rightSpan) images
	 * the curFrame will return buffer[leftSpan]
	 */
	void init();

	/** if index is outside of the [minNonNullIndex, maxNonNullIndex], then return true
	 */
	bool isIndexFrameNull(int index) const ;

private:
	UnifiedImageSequenceReader* uisr;

	int leftSpan;
	int rightSpan;
	int bufferSize; //leftSpan + rightSpan + 1

	int curIndex;

	std::vector<IplImage*> buffer;

	int bufferIndex;  //buffer[bufferIndex] is the current frame

	//if index is outside of the [minNonNullIndex, maxNonNullIndex], then return NULL
	int minNonNullIndex;
	int maxNonNullIndex;

	bool arriveEnd;
};

}
#endif // FVISION_IMIO_IMAGE_SEQUENCE_BUFFER_IMPL_H_