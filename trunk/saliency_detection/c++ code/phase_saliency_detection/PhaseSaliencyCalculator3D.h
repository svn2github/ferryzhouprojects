#ifndef PHASE_SALIENCY_CALCULATOR_3D_H_
#define PHASE_SALIENCY_CALCULATOR_3D_H_

#include <fftw3.h>
#include <cv.h>

#include <vector>

//input a set of (resized) images
//output a set of grayscale saliency images
class PhaseSaliencyCalculator3D {
public:
	virtual ~PhaseSaliencyCalculator3D(void) {}
	virtual void setMotionStep(int step) = 0;
	virtual void setWidthHeightLength(int width, int height, int length) = 0;
	virtual void compute(const std::vector<IplImage*>& srcs, std::vector<IplImage*>& dsts) = 0;

};

//
class GrayScalePhaseSaliencyCalculator3D : public PhaseSaliencyCalculator3D {
public:
	GrayScalePhaseSaliencyCalculator3D();
	~GrayScalePhaseSaliencyCalculator3D();

public:
	void setMotionStep(int step);
	void setWidthHeightLength(int width, int height, int length);

	void compute(const std::vector<IplImage*>& srcs, std::vector<IplImage*>& dsts);

private:
	std::vector<IplImage*> getGrayScaleSrcs(const std::vector<IplImage*>& srcs);
	bool isInitialized() const;

	void clean();
	void initializeWith(int width, int height, int length);

private:
	int motionStep;
	int width;
	int height;
	int length;
	int pixelSize;

	std::vector<IplImage*> outputBuffer;
	std::vector<IplImage*> grayScaleSrcsBuffer;

	fftw_complex* data_in;
	fftw_complex* fft;
	fftw_complex* ifft;

	fftw_plan plan_f;
	fftw_plan plan_b;

};

#endif // PHASE_SALIENCY_CALCULATOR_3D_H_
