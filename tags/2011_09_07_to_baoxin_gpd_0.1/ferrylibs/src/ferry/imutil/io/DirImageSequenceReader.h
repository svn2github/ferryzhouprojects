#pragma once

#include <iostream>
#include <ferry/sysutil/JFile.h>
#include <ferry/util/StringUtil.h>

#include "ImageSequenceReader.h"

using namespace std;
using namespace ferry::sysutil;
using namespace ferry::util;

namespace ferry { namespace imutil {
	namespace io {

/*!
* read all images inside a directory
*/
class DirImageSequenceReader : public ImageSequenceReader {

public:
	DirImageSequenceReader(const char* path, const char* suffix = NULL) : ImageSequenceReader(path) {
		JFile jfile;
		this->path = new char[strlen(path) + 1];
		strcpy(this->path, path);

		char filename[300];
		sprintf(filename, "%s/*.*", path);
		vector<string> allfiles = jfile.listFiles(filename);
		if (allfiles.size() == 0) failed = true;
		else failed = false;
		files = allfiles;
		curIndex = 0;
		curFrame = NULL;
	}

	~DirImageSequenceReader(void) {
	}

public:
	bool isFailed() {
		return failed;
	}

	IplImage* nextFrame() {
		
		while (curIndex < (int)files.size()) {
			char filepath[300];
			sprintf(filepath, "%s/%s", path, files[curIndex].c_str());
			
			string filenameBuf;
			sprintf(filename, "%s", getFileName(files[curIndex].c_str(), filenameBuf));

			//cout<<"filepath: "<<filepath<<endl;
			curFrame = cvLoadImage(filepath, 1);
			curIndex++;
			if (curFrame != NULL) return curFrame;
			else continue;
		}
		return NULL;
	}

	void close() {
	}

	char* getFilename() {
		return filename;
	}

private:
	char* path;
	vector<string> files;
	int curIndex;
	IplImage* curFrame;
	bool failed;
	char filename[300];
};

} } }