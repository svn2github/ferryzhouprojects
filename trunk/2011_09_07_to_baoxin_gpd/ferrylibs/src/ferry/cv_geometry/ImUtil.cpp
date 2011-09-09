#include ".\imutil.h"

#include <cassert>

namespace ferry {
	namespace cv_geometry {

IplImage* concatenateImage(const IplImage* src1, const IplImage* src2) {

	assert(src1 != NULL && src2 != NULL);
	CvSize size = cvGetSize(src1);
	CvSize size2 = cvGetSize(src2);
	assert(size.height == size2.height && size.width == size2.width);

	IplImage* stereoImage = cvCreateImage(cvSize(size.width * 2, size.height), 8, src1->nChannels);
	
	cvSetImageROI(stereoImage, cvRect(0, 0, size.width, size.height));
	cvCopy(src1, stereoImage);

	cvSetImageROI(stereoImage, cvRect(size.width, 0, size.width, size.height));
	cvCopy(src2, stereoImage);

	cvSetImageROI(stereoImage, cvRect(0, 0, size.width * 2, size.height));

	return stereoImage;
}

void addWeightedWithBlack(const IplImage* src1, double alpha, const IplImage* src2, double beta, IplImage* dst) {
	cvZero(dst);
	IplImage* overlapMask3 = cvCreateImage(cvGetSize(src1), 8, src1->nChannels);
	cvMul(src1, src2, overlapMask3, 0.01);
	//cvNamedWindow("overlap 0", 1);
	//cvShowImage("overlap 0", overlapMask3);
	
	IplImage* overlapMask = cvCreateImage(cvGetSize(src1), 8, 1);
	cvCvtColor(overlapMask3, overlapMask, CV_BGR2GRAY);
	//cvNamedWindow("overlap 1", 1);
	//cvShowImage("overlap 1", overlapMask);

	cvSubS(overlapMask, cvScalar(10), overlapMask);
	//cvNamedWindow("overlap2", 1);
	//cvShowImage("overlap2", overlapMask);

	IplImage* src12 = cvCreateImage(cvGetSize(src1), 8, src1->nChannels);
	cvCopy(src1, src12, overlapMask);

	IplImage* src11 = cvCreateImage(cvGetSize(src1), 8, src1->nChannels);
	cvSub(src1, src12, src11);

	IplImage* src22 = cvCreateImage(cvGetSize(src2), 8, src1->nChannels);
	cvCopy(src2, src22, overlapMask);
	IplImage* src21 = cvCreateImage(cvGetSize(src2), 8, src2->nChannels);
	cvSub(src2, src22, src21);
	
	cvAddWeighted(src12, alpha, src22, beta, 0, dst);

	cvAdd(src11, dst, dst);

	cvAdd(src21, dst, dst);

	cvReleaseImage(&src11);
	cvReleaseImage(&src12);
	cvReleaseImage(&src21);
	cvReleaseImage(&src22);
	cvReleaseImage(&overlapMask3);
	cvReleaseImage(&overlapMask);
}

void addOverlapWithBlack(const IplImage* src, IplImage* dst) {
	IplImage* overlapMask3 = cvCreateImage(cvGetSize(src), 8, src->nChannels);
	cvMul(src, dst, overlapMask3, 0.01);
	//cvNamedWindow("overlap0", 1);
	//cvShowImage("overlap0", overlapMask3);
	
	IplImage* overlapMask = cvCreateImage(cvGetSize(src), 8, 1);
	cvCvtColor(overlapMask3, overlapMask, CV_BGR2GRAY);

	//cvNamedWindow("overlap", 1);
	//cvShowImage("overlap", overlapMask);
	cvSubS(overlapMask, cvScalar(10), overlapMask);
	//cvNamedWindow("overlap2", 1);
	//cvShowImage("overlap2", overlapMask);

	IplImage* src12 = cvCreateImage(cvGetSize(src), 8, src->nChannels);
	cvCopy(src, src12, overlapMask);

	IplImage* src11 = cvCreateImage(cvGetSize(src), 8, src->nChannels);
	cvSub(src, src12, src11);

	cvAdd(src11, dst, dst);

	cvReleaseImage(&src12);
	cvReleaseImage(&src11);
	cvReleaseImage(&overlapMask3);
	cvReleaseImage(&overlapMask);
}

	}
}