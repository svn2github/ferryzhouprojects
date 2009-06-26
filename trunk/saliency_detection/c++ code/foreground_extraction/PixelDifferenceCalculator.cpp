#include "PixelDifferenceCalculator.h"


void computeImageDifference(const IplImage* src1, const IplImage* src2, IplImage* dst, PixelDifferenceCalculator* calculator) {
	assert (src1->nChannels == 3 && src2->nChannels == 3 && dst->nChannels == 1);

	const uchar* data1 = (const uchar*)src1->imageData;
	const uchar* data2 = (const uchar*)src2->imageData;
	uchar* data3 = (uchar*)dst->imageData;
	
	int index = 0;
	int offset = 0;
	for (int i = 0; i < src1->height; i++) {
		offset = src1->widthStep * i;
		for (int j = 0; j < src1->width; j++) {
			index = offset + j * src1->nChannels;
			data3[i * dst->widthStep + j] = calculator->compute(data1 + index, data2 + index);
		}
	}
}