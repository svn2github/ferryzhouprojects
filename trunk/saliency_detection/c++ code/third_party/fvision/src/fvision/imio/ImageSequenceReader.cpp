#include ".\imagesequencereader.h"

namespace fvision {

ImageSequenceReader::ImageSequenceReader(const char* path) {
	this->path = new char[strlen(path) + 1];
	strcpy(this->path, path);
}

ImageSequenceReader::~ImageSequenceReader(void) {
	if (path != NULL) {
		delete path;
		path = NULL;
	}
}


}
