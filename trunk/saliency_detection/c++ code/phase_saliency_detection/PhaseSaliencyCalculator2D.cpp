#include "PhaseSaliencyCalculator2D.h"

#include "phase_saliency_detection_utils.h"

#include <fftw3.h>

GrayScalePhaseSaliencyCalculator2D::GrayScalePhaseSaliencyCalculator2D() {
	buffer = NULL;	
}

GrayScalePhaseSaliencyCalculator2D::~GrayScalePhaseSaliencyCalculator2D(void) {
	if (buffer != NULL) cvReleaseImage(&buffer);
}

IplImage* GrayScalePhaseSaliencyCalculator2D::compute(const IplImage* im) {

	const IplImage* grayScaleImage = getGrayScaleImage(im);
	IplImage* saliencyImage = cvCreateImage(cvGetSize(im), 8, 1);
	cvZero(saliencyImage);

	/* get image properties */
	int width  	  = grayScaleImage->width;
	int height 	  = grayScaleImage->height;
	int step	  = grayScaleImage->widthStep;
	uchar* img1_data = ( uchar* ) grayScaleImage->imageData;
	uchar* img2_data = ( uchar* ) saliencyImage->imageData;    

	/*initialize arrays for fftw operations */
	fftw_complex* data_in = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * width * height );
	fftw_complex* fft     = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * width * height );
	fftw_complex* pfft    = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * width * height ); //phase
	fftw_complex* ipfft   = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * width * height ); //

	/* create plans */
	fftw_plan plan_f  = fftw_plan_dft_1d( width * height, data_in, fft,   FFTW_FORWARD,  FFTW_ESTIMATE );
	fftw_plan plan_ip = fftw_plan_dft_1d( width * height, pfft,    ipfft, FFTW_BACKWARD, FFTW_ESTIMATE );

	/* load grayScaleImage's data to fftw input */
	for( int i = 0, k = 0 ; i < height ; i++ ) {
		for( int j = 0 ; j < width ; j++ ) {
			data_in[k][0] = ( double )img1_data[i * step + j];
			data_in[k][1] = 0.0;
			k++;
		}
	}

	/* perform FFT */
	fftw_execute( plan_f );

	//extract phase, i.e., normalize each complex number by dividing the power spectrum
	extractPhase(fft, pfft, width * height);

	/* perform IFFT */
	fftw_execute( plan_ip );

	double N = width * height;

	/* normalize IFFT result */
	for(int i = 0 ; i < ( width * height ) ; i++ ) {
		ipfft[i][0] /= N;
		ipfft[i][1] /= N;
	}

	double maxs = getComplexArrayMaxAbs(ipfft, N);
	/* copy IFFT result to img2's data */
	for(int i = 0, k = 0 ; i < height ; i++ ) {
		for(int j = 0 ; j < width ; j++ ) {
			double s = (ipfft[k][0] * ipfft[k][0] + ipfft[k][1] * ipfft[k][1]);
			img2_data[i * step + j] = ( uchar ) (s * 255 / maxs);
			k++;
		}
	}

	return saliencyImage;
}

const IplImage* GrayScalePhaseSaliencyCalculator2D::getGrayScaleImage(const IplImage* im) {
	const IplImage* grayScaleImage = NULL;
	if (im->nChannels == 1) grayScaleImage = im;
	else {
		if (buffer == NULL) {
			buffer = cvCreateImage(cvGetSize(im), 8, 1);
		} else if (buffer->width != im->width || buffer->height != im->height) {
			cvReleaseImage(&buffer);
			buffer = cvCreateImage(cvGetSize(im), 8, 1);
		}
		cvCvtColor(im, buffer, CV_BGR2GRAY);
		grayScaleImage = buffer;
	}
	return grayScaleImage;
}