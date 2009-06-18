#ifndef FVISION_IMIO_IMAGE_SEQUENCE_BUFFER_H_
#define FVISION_IMIO_IMAGE_SEQUENCE_BUFFER_H_

#include <cv.h>

namespace fvision {

/** Keep a certian images buffer while sequentially reading a image sequence
 * the buffer window is [curIndex - leftSpan, curIndex + rightSpan], images in this window can be retrieved
 * 
 */
class ImageSequenceBuffer {

public:
	ImageSequenceBuffer() {
	}

	virtual ~ImageSequenceBuffer() {
	}

private:
	//non copyable
	ImageSequenceBuffer(const ImageSequenceBuffer& rhs) {} 
	ImageSequenceBuffer& operator= (const ImageSequenceBuffer& rhs) {} 

public:

	/** read next frame
	 * return NULL if end
	 * increase curIndex
	 */
	virtual IplImage* nextFrame() = 0;

	virtual int getCurIndex() const = 0;

	virtual IplImage* curFrame() const = 0;

	///return NULL if the frame is not available or out of buffer window
	virtual IplImage* getFrame(int index) const = 0;

	/** assert offset >= -leftSpan && offset <= rightSpan
	 */
	virtual IplImage* curFrameWithOffset(int offset) const = 0;
};

}
#endif // FVISION_IMIO_IMAGE_SEQUENCE_BUFFER_H_