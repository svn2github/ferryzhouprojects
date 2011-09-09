#pragma once

#include <string>
#include <iostream>

#include <ferry/util/StringUtil.h>

#include "ImageSequenceReader.h"
#include "AVIImageSequenceReader.h"
#include "StillImageSequenceReader.h"
#include "DirImageSequenceReader.h"

using namespace std;
using namespace ferry::util;

namespace ferry { namespace imutil {
	namespace io {

/*!
* read image sequences dependent on suffix
*/
class UnifiedImageSequenceReader : public ImageSequenceReader {

public:
	static enum FileType {STILL, AVI, FOLDER};

public:
	UnifiedImageSequenceReader(const char* path) : ImageSequenceReader(path) {
		string dir, filenamestr, suffix;
		decomposePath(path, dir, filenamestr, suffix);
		if (suffix.length() == 0) { //dir
			cout<<"dir ..."<<endl;
			type = FOLDER;
			pisr = new DirImageSequenceReader(path);
		} else if (strcmp(suffix.c_str(), "avi") == 0) {
			cout<<"avi ..."<<endl;
			type = AVI;
			pisr = new AVIImageSequenceReader(path);
		} else { //regard as still image
			cout<<"still ..."<<endl;
			type = STILL;
			pisr = new StillImageSequenceReader(path);
		}
	}

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

	char* getFilename() {
		return pisr->getFilename();
	}

	FileType getType() {
		return type;
	}

private:
	ImageSequenceReader* pisr;

	FileType type;
};

} } }