#pragma once

#include "../ImageSequenceReader.h"

#include <iostream>

#include <vector>
#include <string>

namespace fvision {

/*!
* read all images inside a directory
*/
class DirImageSequenceReader : public ImageSequenceReader {

public:
	DirImageSequenceReader(const char* path, const char* suffix = NULL);

	virtual ~DirImageSequenceReader(void) {
	}

public:
	bool isFailed() {
		return failed;
	}

	IplImage* nextFrame();

	void close() {
	}

	const char* getFilename() const {
		return filename;
	}

private:
	char* path;
	std::vector<std::string> files;
	int curIndex;
	IplImage* curFrame;
	bool failed;
	char filename[300];
};

}