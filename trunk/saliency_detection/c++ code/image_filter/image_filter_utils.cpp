#include "image_filter_utils.h"

namespace image_filter {

bool isImageHeaderEqual(const IplImage* im1, const IplImage* im2) {
	return (im1->width == im2->width 
		&& im1->height == im2->height 
		&& im1->depth == im2->depth 
		&& im1->nChannels == im2->nChannels
		);	
}

}