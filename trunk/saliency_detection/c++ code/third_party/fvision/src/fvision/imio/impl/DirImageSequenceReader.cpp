#include ".\dirimagesequencereader.h"

#include <fvision/utils/file_utils.h>
#include <fvision/utils/string_utils.h>

#include <highgui.h>

using namespace std;

namespace fvision{

DirImageSequenceReader::DirImageSequenceReader(const char* path, const char* suffix) : ImageSequenceReader(path) {

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

IplImage* DirImageSequenceReader::nextFrame() {
	
	while (curIndex < (int)files.size()) {
		char filepath[300];
		sprintf(filepath, "%s/%s", path, files[curIndex].c_str());
		
		string filenameBuf;
		sprintf(filename, "%s", getFileName(files[curIndex].c_str(), filenameBuf));

		//cout<<"filepath: "<<filepath<<endl;
		if (curFrame != NULL) {
			cvReleaseImage(&curFrame);
			curFrame = NULL;
		}
		curFrame = cvLoadImage(filepath, 1);
		curIndex++;
		if (curFrame != NULL) return curFrame;
		else continue;
	}
	return NULL;
}


}