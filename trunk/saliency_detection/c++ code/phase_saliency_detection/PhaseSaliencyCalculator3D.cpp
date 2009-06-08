#include "PhaseSaliencyCalculator3D.h"

#include "phase_saliency_detection_utils.h"
#include <cassert>
#include <iostream>

using namespace std;

void GrayScalePhaseSaliencyCalculator3D::compute(const std::vector<IplImage*>& srcs, std::vector<IplImage*>& dsts) {

	assert(isInitialized());
	assert((int)srcs.size() == length);

	vector<IplImage*> grayScaleSrcs = getGrayScaleSrcs(srcs);

	//setup input data
	int step = grayScaleSrcs[0]->widthStep;
	int k = 0;
	for (int t = 0; t < length; t++) {
		uchar* im_data = (uchar*)(grayScaleSrcs[t]->imageData);
		int nt = t - motionStep;
		if (nt < 0) nt = 0;
		uchar* pim_data = (uchar*)(grayScaleSrcs[nt]->imageData);
		for(int i = 0; i < height ; i++ ) {
			for( int j = 0 ; j < width ; j++ ) {
				//data_in[k][0] = (double)im_data[i * step + j];
				//data_in[k][1] = 0;//(double)im_data[i * step + j];
				data_in[k][0] = (double)abs(im_data[i * step + j] - pim_data[i * step + j]);
				data_in[k][1] = (double)im_data[i * step + j];
				k++;
			}
		}
	}

	fftw_execute(plan_f);
	normalizeComplexArray(fft, pixelSize);
	fftw_execute(plan_b);
	
	//setup output data
	divideComplexArray(ifft, pixelSize, pixelSize);

	//double maxs = getComplexArrayMaxAbs(ifft, pixelSize);
	//cout<<"maxs: "<<maxs<<endl;
	k = 0;
	//step = outputBuffer[0]->widthStep;
	for (int t = 0; t < length; t++) {
		//uchar* im_data = (uchar*)(outputBuffer[t]->imageData);
		for(int i = 0; i < height ; i++ ) {
			for( int j = 0 ; j < width ; j++ ) {
				double s = (ifft[k][0] * ifft[k][0] + ifft[k][1] * ifft[k][1]);
				//cvSetReal2D(outputBuffer[t], i, j, s * 30/ maxs);
				cvSetReal2D(outputBuffer[t], i, j, s * 30000); //maxs = 0.001
				k++;
			}
		}
	}

	dsts = outputBuffer;
}

void GrayScalePhaseSaliencyCalculator3D::setMotionStep(int step) {
	assert(step > 0);
	this->motionStep = step;
}

std::vector<IplImage*> GrayScalePhaseSaliencyCalculator3D::getGrayScaleSrcs(const std::vector<IplImage*>& srcs) {
	std::vector<IplImage*> gsrcs(srcs.size());
	for (size_t i = 0; i < srcs.size(); i++) {
		if (srcs[i]->nChannels == 1) gsrcs[i] = srcs[i];
		else {
			cvCvtColor(srcs[i], grayScaleSrcsBuffer[i], CV_BGR2GRAY);
			gsrcs[i] = grayScaleSrcsBuffer[i];
		}
	}
	return gsrcs;
}

GrayScalePhaseSaliencyCalculator3D::GrayScalePhaseSaliencyCalculator3D() {
	motionStep = 1;
	width = -1;
	height = -1;
	length = -1;
	pixelSize = 0;
	data_in = NULL;
	fft = NULL;
	ifft = NULL;
}

GrayScalePhaseSaliencyCalculator3D::~GrayScalePhaseSaliencyCalculator3D() {
	clean();
}

bool GrayScalePhaseSaliencyCalculator3D::isInitialized() const {
	return motionStep > 0 && width > 0 && height > 0 && length > 0;
}

void GrayScalePhaseSaliencyCalculator3D::setWidthHeightLength(int width, int height, int length) {
	assert (width > 0 && height > 0 && length > 0);
	if (data_in != NULL) clean();
	initializeWith(width, height, length);
}

void GrayScalePhaseSaliencyCalculator3D::initializeWith(int width, int height, int length) {

	this->width = width;
	this->height  = height;
	this->length = length;

	pixelSize = width * height * length;
	int size = sizeof(fftw_complex) * pixelSize;

	data_in = (fftw_complex*)fftw_malloc(size);
	fft = (fftw_complex*)fftw_malloc(size);
	ifft = (fftw_complex*)fftw_malloc(size);

	plan_f = fftw_plan_dft_1d( pixelSize, data_in, fft,  FFTW_FORWARD,  FFTW_ESTIMATE );
	plan_b = fftw_plan_dft_1d( pixelSize, fft,     ifft, FFTW_BACKWARD, FFTW_ESTIMATE );

	outputBuffer.resize(length);
	grayScaleSrcsBuffer.resize(length);
	CvSize imSize = cvSize(width, height);
	for (int i = 0; i < length; i++) {
		outputBuffer[i] = cvCreateImage(imSize, IPL_DEPTH_32F, 1);
		grayScaleSrcsBuffer[i] = cvCreateImage(imSize, 8, 1);
	}
}

void GrayScalePhaseSaliencyCalculator3D::clean() {
	if (data_in != NULL) {
		fftw_free( data_in );
		fftw_free( fft );
		fftw_free( ifft );
		fftw_destroy_plan( plan_f );
		fftw_destroy_plan( plan_b );
		for (size_t i = 0; i < outputBuffer.size(); i++) {
			cvReleaseImage(&(outputBuffer[i]));
		}
		for (size_t i = 0; i < grayScaleSrcsBuffer.size(); i++) {
			cvReleaseImage(&(grayScaleSrcsBuffer[i]));
		}
	}
}