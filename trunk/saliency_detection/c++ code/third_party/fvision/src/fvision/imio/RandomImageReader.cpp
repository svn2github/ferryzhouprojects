#include "RandomImageReader.h"

#include <highgui.h>

namespace fvision {

RandomImageReader::RandomImageReader(const PathParams& pathParams) {
	this->pathParams = pathParams;
}

RandomImageReader::~RandomImageReader(void)
{
}

IplImage* RandomImageReader::getImage(int frameId) {
	return cvLoadImage(pathParams.getPath(frameId).c_str());
}
}