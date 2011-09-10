#pragma once

#include <cassert>
#include <algorithm>

#include "ImageFilter.h"

using namespace std;

namespace ferry { namespace imutil { namespace filter {

class SieveFilter : public ImageFilter {

public:
	//patch size: (2 * xhwin + 1) x (2 * yhwin + 1)
	SieveFilter(int xhwin, int yhwin, double hole, int iterations = 1) {
		assert(xhwin >= 0 && yhwin >= 0 && hole >= 0 && iterations > 0);

		this->xhwin = xhwin;
		this->yhwin = yhwin;
		this->hole = hole;
		this->iterations = iterations;
	}

	~SieveFilter(void) {
	}

private:
	template<typename T>
	IplImage* processTemplate(T*, IplImage* src, IplImage* dst) {

		int width = src->width;
		int height = src->height;
		//int widthStep = src->widthStep;

		T* data = (T*)src->imageData;
		T* ddata = (T*)dst->imageData;
		int nChannels = src->nChannels;
		int x1, x2, y1, y2;

		//int patchSize = (2 * xhwin + 1) * (2 * yhwin + 1);
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

					double dcsum = 0;
					for (int pi = x1; pi <= x2; pi++) {
						for (int pj = y1; pj <= y2; pj++) {
							T t = data[(pj * width + pi) * nChannels + k];
							if (abs(c - t) < hole) {
								dcsum += t;
							} else {
								dcsum += c;
							}
						}
					}
					int size = ((x2 - x1 + 1) * (y2 - y1 + 1));
					ddata[offset + i * nChannels + k] = (T)(dcsum / size);
				}

			}
		}

		return dst;				
	}

public:
	IplImage* process(IplImage* src, IplImage* dst) {
		IplImage* buffer = cvCloneImage(src);
		for (int i = 0; i < iterations; i++) {
			processSinglePass(buffer, dst);
			cvCopy(dst, buffer);
		}
		cvReleaseImage(&buffer);
		return dst;
	}

	IplImage* processSinglePass(IplImage* src, IplImage* dst);
private:
	int xhwin;
	int yhwin;
	double hole;
	int iterations;
};

} } }
