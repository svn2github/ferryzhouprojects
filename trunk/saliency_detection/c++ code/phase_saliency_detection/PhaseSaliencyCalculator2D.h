#ifndef PHASE_SALIENCY_CALCULATOR_2D_H_
#define PHASE_SALIENCY_CALCULATOR_2D_H_

#include <cv.h>

//input a (resized) color image
//output a grayscale saliency map
class PhaseSaliencyCalculator2D {
public:
	virtual ~PhaseSaliencyCalculator2D(void) {}
	virtual IplImage* compute(const IplImage* im) = 0;
};

class GrayScalePhaseSaliencyCalculator2D : public PhaseSaliencyCalculator2D {
public:
	GrayScalePhaseSaliencyCalculator2D();
	~GrayScalePhaseSaliencyCalculator2D(void);

	IplImage* compute(const IplImage* im);

private:
	const IplImage* getGrayScaleImage(const IplImage* im);

private:
	IplImage* buffer;
};

#endif // PHASE_SALIENCY_CALCULATOR_2D_H_
