/************************************************************************
* Sharp Labs of America, Inc.
* Copyright 2006. All rights reserved
************************************************************************
*/

/*!
************************************************************************
* \file ImageHistogram.cpp
* \brief implementation for ImageHistogram
* \author Jin Zhou               <jzhou@sharplabs.com>   \n
*  
************************************************************************
*/

#include ".\imagehistogram.h"

namespace ferry { namespace imutil { namespace core {

ImageHistogram::ImageHistogram(IplImage* frame, int sampleStep) {
	readImage(frame, sampleStep);
}

ImageHistogram::~ImageHistogram(void)
{
}

//currently, just allow <= 8 depth
void ImageHistogram::readImage(IplImage* frame, int sampleStep) {

	if (frame == NULL) return;

	clear();

	this->nChannels = frame->nChannels;
	this->depth = frame->depth;

	CvSize nSize = cvGetSize(frame);

	if (this->depth <= 8) {

		uchar* data = (uchar*)frame->imageData;

		for (int j = 0; j < nSize.height; j += sampleStep) {
			int offset = j * frame->widthStep;
			for (int i = 0; i < nSize.width; i += sampleStep) {
				uchar v = 0; //brightness
				for (int k = 0; k < nChannels; k ++) {
					uchar c = data[offset + i * nChannels + k];
					
					histogram[k].data[c] ++;

					if (c > v) v = c;
				}
				histogram[3].data[v] ++;
			}
		}
	}
}

} } }