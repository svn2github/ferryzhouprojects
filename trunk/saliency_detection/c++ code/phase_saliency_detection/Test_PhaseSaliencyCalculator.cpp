#include "Test_PhaseSaliencyCalculator.h"

#include "PhaseSaliencyCalculator2D.h"
#include "PhaseSaliencyCalculator3D.h"

#include <highgui.h>

#include <iostream>

using namespace std;

Test_PhaseSaliencyCalculator::Test_PhaseSaliencyCalculator(void)
{
}

Test_PhaseSaliencyCalculator::~Test_PhaseSaliencyCalculator(void)
{
}

void Test_PhaseSaliencyCalculator::test2d() {
	IplImage* src = cvLoadImage("H:\\zj\\data\\surveillance\\SaliencyPatterns\\4.jpg");
	CvSize imSize = cvGetSize(src);
	CvSize newSize = cvSize(imSize.width / 8, imSize.height / 8);
	IplImage* resizedImage = cvCreateImage(newSize, src->depth, src->nChannels);
	cvResize(src, resizedImage);
	PhaseSaliencyCalculator2D* psc = new GrayScalePhaseSaliencyCalculator2D();
	IplImage* saliencyImage = psc->compute(resizedImage);

	IplImage* os = cvCreateImage(imSize, 8, 1);
	cvResize(saliencyImage, os);

	cvNamedWindow("src");
	cvShowImage("src", src);
	cvNamedWindow("saliency");
	cvShowImage("saliency", saliencyImage);
	cvSaveImage("saliency.png", saliencyImage);
	cvNamedWindow("nsaliency");
	cvShowImage("nsaliency", os);
}

void Test_PhaseSaliencyCalculator::test3d() {
	const char* pathRegex = "H:\\zj\\data\\surveillance\\pets2000\\%04d.jpg";
	const char* outputRegex = "H:\\zj\\data\\surveillance\\results\\phase\\pets2000\\im_%04d.jpg";
	const int PATH_SIZE = 300;
	char path[PATH_SIZE];

	int length = 300;
	int motionStep = 2;
	int divideN = 8;
	int startIndex = 600;
	std::vector<IplImage*> srcs(length);

	sprintf_s(path, PATH_SIZE, pathRegex, 0);
	IplImage* im0 = cvLoadImage(path);
	if (im0 == NULL) {
		cout<<"can not load "<<path<<" !"<<endl;
		exit(1);
	}
	int owidth = im0->width;
	int oheight = im0->height;
	cvReleaseImage(&im0);

	int width  = owidth / divideN;
	int height = oheight / divideN;

	IplImage* srcBuf = cvCreateImage(cvSize(width, height), 8, 1);

	for (int i = 0; i < length; i++) {
		sprintf_s(path, PATH_SIZE, pathRegex, i + startIndex);
		IplImage* im = cvLoadImage(path, 0);
		cvResize(im, srcBuf);
		srcs[i] = cvCloneImage(srcBuf);
		cvReleaseImage(&im);
	}

	PhaseSaliencyCalculator3D* psc = new GrayScalePhaseSaliencyCalculator3D();
	psc->setMotionStep(motionStep);
	psc->setWidthHeightLength(width, height, length);

	std::vector<IplImage*> dsts;
	psc->compute(srcs, dsts);

	cvNamedWindow("saliency");
	IplImage* dstBuf = cvCreateImage(cvSize(width, height), 8, 1);

	for (int i = 0; i < length; i++) {
		sprintf_s(path, PATH_SIZE, outputRegex, i + startIndex);
		cvSmooth(dsts[i], dsts[i], CV_GAUSSIAN, 7);
		cvConvertScale(dsts[i], dstBuf, 255);
		cvSaveImage(path, dstBuf);
		cvShowImage("saliency", dstBuf);
		cvWaitKey(30);
	}

}

