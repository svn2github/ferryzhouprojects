#include "ImageTypeInfo.h"

namespace image_filter {

ImageTypeInfo::ImageTypeInfo(void)
{
}

ImageTypeInfo::~ImageTypeInfo(void)
{
}

ImageTypeInfo::ImageTypeInfo(const CvSize& size, int depth, int channels) {
	this->size = size;
	this->depth = depth;
	this->channels = channels;
}

ImageTypeInfo ImageTypeInfo::getTypeInfo(const IplImage* im) {
	return ImageTypeInfo(cvGetSize(im), im->depth, im->nChannels);
}

bool ImageTypeInfo::operator== (const ImageTypeInfo& rhs) {
	return (this->size.height == rhs.size.height
		&&  this->size.width == rhs.size.width
		&&  this->depth == rhs.depth
		&&  this->channels == rhs.channels
		);
}

bool ImageTypeInfo::imageTypeEquals(const IplImage* im1, const IplImage* im2) {
	return getTypeInfo(im1) == getTypeInfo(im2);
}

}