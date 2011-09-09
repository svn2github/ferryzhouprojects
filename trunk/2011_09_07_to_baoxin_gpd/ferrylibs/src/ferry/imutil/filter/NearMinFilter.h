#pragma once

#include <cassert>
#include <algorithm>

#include "ImageFilter.h"

using namespace std;

namespace ferry { namespace imutil { namespace filter {

class NearMinFilter : public ImageFilter {

public:
	NearMinFilter(int xhwin, int yhwin) {
		assert(xhwin >= 0 && yhwin >= 0);

		this->xhwin = xhwin;
		this->yhwin = yhwin;
	}

	~NearMinFilter(void) {
	}

public:
	IplImage* process(IplImage* src, IplImage* dst) {
		return processSinglePass(src, dst);
	}

	IplImage* processSinglePass(IplImage* src, IplImage* dst) {
		if (src->depth == IPL_DEPTH_8U) return processTemplate ((uchar*)0, src, dst);
		if (src->depth == IPL_DEPTH_8S) return processTemplate((char*)0, src, dst);
		if (src->depth == IPL_DEPTH_16U) return processTemplate((ushort*)0, src, dst);
		if (src->depth == IPL_DEPTH_16S) return processTemplate((short*)0, src, dst);
		if (src->depth == IPL_DEPTH_32S) return processTemplate((float*)0, src, dst);
		if (src->depth == IPL_DEPTH_32F) return processTemplate((float*)0, src, dst);
		if (src->depth == IPL_DEPTH_64F) return processTemplate((double*)0, src, dst);
		
		return dst;
	}

private:
	template<typename T>
	IplImage* processTemplate(T*, IplImage* src, IplImage* dst) {

		int width = src->width;
		int height = src->height;

		T* data = (T*)src->imageData;
		T* ddata = (T*)dst->imageData;
		int nChannels = src->nChannels;
		int x1, x2, y1, y2;

		for (int j = 0; j < height; j++) {

			int offset = j * width * nChannels;

			for (int i = 0; i < width; i++) {

					x1 = i - xhwin;
					if (x1 < 0) x1 = 0;
					x2 = i + xhwin;
					if (x2 >= width) x2 = width - 1;
					y1 = j - yhwin;
					if (y1 < 0) y1 = 0;
					y2 = j + yhwin;
					if (y2 >= height) y2 = height -1;

				for (int k = 0; k < nChannels; k ++) {
					T c = data[offset + i * nChannels + k];	

					T minv = c;
					for (int pi = x1; pi <= x2; pi++) {
						for (int pj = y1; pj <= y2; pj++) {
							//int index = (pj * width + pi) * nChannels + k;
							//T t = data[index];
							T t = data[(pj * width + pi) * nChannels + k];
							if (t < minv) {
								minv = t;
							}
						}
					}

					ddata[offset + i * nChannels + k] = minv;
				}
			}
		}

		return dst;				
	}

private:
	int xhwin;
	int yhwin;
	double hole;
	int iterations;

};

} } }
