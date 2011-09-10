/************************************************************************
* Sharp Labs of America, Inc.
* Copyright 2006. All rights reserved
************************************************************************
*/

/*!
************************************************************************
* \file ImageSingleChannelHistogram.h
* \brief class for histogram, single channel. is used by ImageHistogram
* \author Jin Zhou               <jzhou@sharplabs.com>   \n
*  
************************************************************************
*/

#pragma once

#include <iostream>
#include <cmath>
#include <cassert>

#include "cv.h"

using namespace std;

namespace ferry { namespace imutil { namespace core {

class ImageSingleChannelHistogram
{
public:
	ImageSingleChannelHistogram(void) {
		depth = 8;
		hdata = NULL;
		clear();
	}

	~ImageSingleChannelHistogram(void);

public:
	void clear() {
		shortComputed = false;
		normalized = false;
		nPixels = -1;
		for (int i = 0; i < 256; i++) {
			data[i] = 0;
			ndata[i] = 0;
			sdata[i] = 0;
		}
	}

public:
	void setDepth(int depth) {
		assert(depth > 0 && depth <= 16);
		this->depth = depth;
	}

	//currently, just allow <= 8 depth
	void readImageChannel(IplImage* im, int channelIndex) {
		assert(channelIndex < im->nChannels);
		assert(im->depth == IPL_DEPTH_8U || im->depth == IPL_DEPTH_8S);

		CvSize nSize = cvGetSize(im);
		uchar* cdata = (uchar*)im->imageData;

		int rowOffset = 0;
		for (int j = 0; j < nSize.height; j++) {
			rowOffset += im->widthStep;
			int offset = rowOffset;
			for (int i = 0; i < nSize.width; i++) {
				offset += i * im->nChannels;
				uchar c = cdata[offset + channelIndex];	
				data[c] ++;
			}
		}
	}

public:
	void normalize() {
		if (shortComputed) {
			int MAX = 0xFFFF;
			for (int i = 0; i < 256; i++) {
				ndata[i] = (double)sdata[i] / MAX;
			}			
		} else {
			int sum = getPixelsNum();
			for (int i = 0; i < 256; i++) {
				ndata[i] = (double)data[i] / sum;
			}
		}
	}

	int getPixelsNum() {
		if (nPixels == -1) {
			nPixels = 0;
			for (int i = 0; i < 256; i++) {
				nPixels += data[i];
			}
		}
		return nPixels;
	}

	double* getNormalizedData() {
		if (!normalized) {
			normalize();
			normalized = true;
		}
		return ndata;
	}

	double* getNormalizedPointer() {
		return ndata;
	}

	const unsigned short* getShortData() {
		if (!shortComputed) {
			int MAX = 0xFFFF;
			int nPixels = getPixelsNum();
			for (int i = 0; i < 256; i++) {
				sdata[i] = (unsigned short)(ceil((double)data[i] / nPixels * MAX));
			}
			shortComputed = true;
		}
		return sdata;
	}

	void setShortComputed(bool b) {
		shortComputed = b;
	}

	void setNormalizeComputed(bool b) {
		normalized = b;
	}

	unsigned short* getShortPointer() {
		return sdata;
	}

	double getNormalizedHistogramValue(int index) {
		if (depth == 8) return ndata[index];
		else {
			int t = (int)pow(2.0, 8 - depth);
			double sum = 0;
			for (int i = 0; i < t; i++) {
				sum += ndata[index * t + i];
			}
			return sum;
		}
	}

	int getCvMax() {
		return (int)pow(2.0, depth) - 1;
	}

public:
	friend ostream& operator<<(ostream& os, const ImageSingleChannelHistogram& imh) {
		for (int i = 0; i < 256; i++) {
			os<<imh.data[i]<<endl;
		}
		return os;
	}

public:
	//raw data
	// <= 8 bits
	int data[256];

	//reserved for high color depth that is larger than 8
	int* hdata;

private:
	//pixel number, equals width * height, initialize to -1, indicating not computed yet.
	int nPixels;

	//normalize to [0, 1]
	bool normalized;
	double ndata[256];

	bool shortComputed;
public:
	//normalized to [0 0xFFFF]
	unsigned short sdata[256];

private:
	//usually 8. also allows [0, 16]
	int depth;

};

} } }
