#pragma once

#include <cv.h>

struct OpticalFlowParticle {
	CvPoint2D32f position;
	CvPoint2D32f velocity; //in <x, y>	
};
