#include "BlankImageSequenceReader.h"

#include <cstdio>

namespace fvision {

BlankImageSequenceReader::~BlankImageSequenceReader(void) {
	if (image != NULL) cvReleaseImage(&image);
}

BlankImageSequenceReader::BlankImageSequenceReader(
	const CvSize& size, 
	int depth, 
	int channels, 
	CvScalar color) 
{
	image = cvCreateImage(size, depth, channels);
	cvSet(image, color);
	index = 0;
	this->color = color;
}

IplImage* BlankImageSequenceReader::nextFrame() {
	sprintf(filename, "%04d", index);
	index++;
	cvSet(image, color);
	return image;
}

}
