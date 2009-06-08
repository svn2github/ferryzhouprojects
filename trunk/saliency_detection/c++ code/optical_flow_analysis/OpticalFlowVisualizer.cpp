#include "OpticalFlowVisualizer.h"

#include <highgui.h>

OpticalFlowVisualizer::OpticalFlowVisualizer(const CvSize& size) {
	flowImage = cvCreateImage(size, 8, 3);
	cvZero(flowImage);
	flowField = cvCreateImage(size, IPL_DEPTH_32F, 3);
	cvZero(flowField);
}

OpticalFlowVisualizer::~OpticalFlowVisualizer(void) {
	delete flowImage;
}

//for each 
void OpticalFlowVisualizer::addFrameParticles(const OpticalFlowParticles* particles) {
	
}