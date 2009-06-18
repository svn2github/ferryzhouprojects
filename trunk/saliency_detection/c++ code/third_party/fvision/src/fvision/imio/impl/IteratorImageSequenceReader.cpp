#include "IteratorImageSequenceReader.h"

#include <highgui.h>

using namespace std;

namespace fvision {

IteratorImageSequenceReader::IteratorImageSequenceReader(ImagePathIterator* iter) {
	this->iter = iter;
	curFrame = NULL;
}

IteratorImageSequenceReader::~IteratorImageSequenceReader(void) {
	delete iter;
	if (curFrame != NULL) cvReleaseImage(&curFrame);
}

IplImage* IteratorImageSequenceReader::nextFrame() {
	if (!iter->hasNext()) return NULL;
	if (curFrame != NULL) cvReleaseImage(&curFrame);
	string path = iter->next();
	filename = iter->getFilename();
	curFrame = cvLoadImage(path.c_str());
	return curFrame;
}

const char* IteratorImageSequenceReader::getFilename() const {
	return filename.c_str();
}

}