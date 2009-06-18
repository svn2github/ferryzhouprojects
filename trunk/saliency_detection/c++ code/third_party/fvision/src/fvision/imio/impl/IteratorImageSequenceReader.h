#ifndef FVISION_IMIO_ITERATOR_IMAGE_SEQUENCE_READER_H_
#define FVISION_IMIO_ITERATOR_IMAGE_SEQUENCE_READER_H_

#include "../ImageSequenceReader.h"
#include "../ImagePathIterator.h"

#include <iostream>

#include <vector>
#include <string>

namespace fvision {

class IteratorImageSequenceReader : public ImageSequenceReader {
public:
	///responsible for deleting iter
	IteratorImageSequenceReader(ImagePathIterator* iter);

	virtual ~IteratorImageSequenceReader(void);

public:
	bool isFailed() { return false; }

	IplImage* nextFrame();

	void close() {}

	const char* getFilename() const;

private:
	ImagePathIterator* iter;
	std::string filename;
	IplImage* curFrame;
};

}
#endif // FVISION_IMIO_ITERATOR_IMAGE_SEQUENCE_READER_H_