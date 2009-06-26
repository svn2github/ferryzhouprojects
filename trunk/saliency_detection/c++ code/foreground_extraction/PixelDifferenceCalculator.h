#ifndef PIXEL_DIFFERENCE_CALCULATOR_H_
#define PIXEL_DIFFERENCE_CALCULATOR_H_

#include <cv.h>

#include <cmath>

class PixelDifferenceCalculator {
public:
	virtual ~PixelDifferenceCalculator(void) {}
	virtual uchar compute(const uchar* c1, const uchar* c2) const = 0;
};

class DistancePixelDifferenceCalculator : public PixelDifferenceCalculator {
public:
	uchar compute(const uchar* c1, const uchar* c2) const {
		float sum = 0;
		float diff = 0;
		for (int i = 0; i < 3; i++) {
			diff = (float)c1[i] - c2[i];
			sum += diff * diff;
		}
		return (uchar)sqrt(sum);
	}
};

void computeImageDifference(const IplImage* src1, const IplImage* src2, IplImage* dst, PixelDifferenceCalculator* calculator);

#endif // PIXEL_DIFFERENCE_CALCULATOR_H_
