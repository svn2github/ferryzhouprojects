#include "image_filter.h"

#include <highgui.h>

#include <iostream>

using namespace image_filter;
using namespace std;

ImageFilter* constructResizeFilter() {
	return ResizeImageFilter::createWithNewRatio(0.5);
}

ImageFilter* constructSequenceFilter() {
	SequenceImageFilter* filter = new SequenceImageFilter();
	filter->addFilter(ResizeImageFilter::createWithNewRatio(0.5));
	filter->addFilter(ResizeImageFilter::createWithNewRatio(0.5));
	filter->addFilter(SmoothImageFilter::create(CV_GAUSSIAN, 5));
	return filter;
}

void main() {

	IplImage* src = cvLoadImage("../../data/Desert Landscape.jpg");

	if (src == NULL) {
		cout<<"can not load image!"<<endl;
		exit(1);
	}

	//ImageFilter* rawFilter = constructResizeFilter();
	ImageFilter* rawFilter = constructSequenceFilter();

	ImageFilter* filter = new SmartImageFilter(rawFilter);

	IplImage* dst = filter->process(src);

	cvNamedWindow("src");
	cvShowImage("src", src);

	cvNamedWindow("dst");
	cvShowImage("dst", dst);

	cvWaitKey(0);
}