/************************************************************************
* Sharp Labs of America, Inc.
* Copyright 2006. All rights reserved
************************************************************************
*/

/*!
************************************************************************
* \file ImageHistogram.h
* \brief class of Frame Histogram, four channels.
* \author Jin Zhou               <jzhou@sharplabs.com>   \n
*  
************************************************************************
*/

#pragma once

#include "cv.h"

#include "ImageSingleChannelHistogram.h"

namespace ferry { namespace imutil { namespace core {

//R/G/B/V
class ImageHistogram
{
public:
	ImageHistogram() {}
	ImageHistogram(IplImage* frame, int sampleStep = 1);
	~ImageHistogram(void);

	void setDepth(int depth) {
		for (int i = 0; i < 4; i++) {
			histogram[i].setDepth(depth);
		}
	}

public:
	int getNChannels() {
		return nChannels;
	}

	void computeShort() {
		for (int i = 0; i < 4; i++) {
			histogram[i].getShortData();
		}
	}

public:
	void readImage(IplImage* frame, int sampleStep = 1);

	int getCvMax() {
		return (int)pow(2.0, depth) - 1;
	}

public:
	ImageSingleChannelHistogram histogram[4];

private:
	void clear() {
		for (int i = 0; i < 4; i++) {
			histogram[i].clear();
		}
	}

private:
	int nChannels;

	//usually 8. also allows [0, 16]
	int depth;

};

} } }
