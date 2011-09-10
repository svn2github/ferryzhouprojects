#include ".\imageutil.h"

namespace ferry { namespace imutil { namespace core {

	//if dst = NULL, create a new image
	//else write to dst and return it
	IplImage* lut(IplImage* src, short* lutmap, IplImage* dst) {

		if (dst == NULL) {
			dst = cvCloneImage(src);
		}

		CvSize nSize = cvGetSize(src);
		uchar* data = (uchar*)src->imageData;
		uchar* ddata = (uchar*)dst->imageData;

		if (src->nChannels == 3) {
			for (int j = 0; j < nSize.height; j++) {
				int offset = j * src->widthStep;
				for (int i = 0; i < nSize.width; i++) {
					for (int k = 0; k < 3; k ++) {
						uchar c = data[offset + i * 3 + k];					
						ddata[offset + i * 3 + k] = (uchar)lutmap[c];
					}
				}
			}
		}

		return dst;		
	}

	bool isHeaderEqual(IplImage* im1, IplImage* im2) {

		if (im1 == im2) return true;
		if (im1 == NULL) return false;
		if (im2 == NULL) return false;

		return (
			im1->width == im2->width
			&& im1->height == im2->height
			&& im1->nChannels == im2->nChannels
			&& im1->depth == im2->depth
			&& im1->origin == im2->origin
			);
	}

/*
	string getImageInfo(IplImage* im) {
		char buf[300];
		sprintf(buf, "size: %d x %d, depth: %d, nChannels: %d, origin: %d", im->width, im->height, im->depth, im->nChannels, im->origin);
		return string(buf);
	}
*/
	void synchronizeBuffer(IplImage* src, IplImage** buffer) {
		if (src == NULL) return;
		if ((*buffer) != NULL) {
			if (isHeaderEqual(src, *buffer)) return;
			else cvReleaseImage(buffer);
		}
		*buffer = cvCloneImage(src);
	}

} } }
