#ifndef FVISION_IMIO_UNIFIED_IMAGE_SEQUENCE_READER_H_
#define FVISION_IMIO_UNIFIED_IMAGE_SEQUENCE_READER_H_

#include "../imio_utils.h"
#include "../ImageSequenceReader.h"

namespace fvision {

/**
 * read image sequences dependent on suffix
 */
class UnifiedImageSequenceReader : public ImageSequenceReader {

public:
	UnifiedImageSequenceReader(const char* path);

	~UnifiedImageSequenceReader(void) {
		delete pisr;
	}


public:
	bool isFailed() {
		return pisr->isFailed();
	}

	IplImage* nextFrame() {
		return pisr->nextFrame();
	}

	void close() {
		pisr->close();
	}

	const char* getFilename() const {
		return pisr->getFilename();
	}

	FileType getType() {
		return type;
	}

private:
	ImageSequenceReader* pisr;

	FileType type;
};

}
#endif // FVISION_IMIO_UNIFIED_IMAGE_SEQUENCE_READER_H_