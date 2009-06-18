#include ".\stillimagesequencereader.h"

#include <highgui.h>
#include <fvision/utils/string_utils.h>

using namespace std;

namespace fvision {

StillImageSequenceReader::StillImageSequenceReader(const char* path) : ImageSequenceReader(path) {
	image = cvLoadImage(path, 1);
	if (image == NULL) failed = true;
	else failed = false;
	picked = false;

	string filenameBuf;
	sprintf_s(this->filename, 200, "%s", getFileName(path, filenameBuf));
}

StillImageSequenceReader::~StillImageSequenceReader(void) {
	if (image != NULL) cvReleaseImage(&image);
}

}