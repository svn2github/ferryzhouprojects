//resize the original image
//model each pixel
//cluster pixel, compute cluster mean
//upgrade background gradually b(t+1) = (1 - w) * b(t) + w * p(t+1)
//make background clusters (up to N)

//the difference of the background pixel value and original image pixel value is the saliency

#include "Test_InitialBackgroundAnalizer.h"

#include <highgui.h>

void main() {
	Test_InitialBackgroundAnalizer tester;
	tester.test();
	cvWaitKey(0);
}