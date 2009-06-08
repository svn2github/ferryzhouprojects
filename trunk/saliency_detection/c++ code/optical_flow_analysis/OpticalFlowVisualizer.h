#pragma once

#include "OpticalFlowParticles.h"

//use a diffusion method
//each cell has a flow value (x, y)
//for each frame
//   multiply all flow with a constant alpha
//   update cell flows using particles
//     for each particle, update the local cell flows using (x', y') = (x, y) + (fx, fy)
//     (fx, fy) = f(r) * (nx, ny), where (nx, ny) is the particle flow
//        r is the distance from the cell to the particle center
class OpticalFlowVisualizer {
public:
	OpticalFlowVisualizer(const CvSize& size);
	~OpticalFlowVisualizer(void);
public:

	void addFrameParticles(const OpticalFlowParticles* particles);

	const IplImage* getImage() const { return flowImage; }

private:
	IplImage* flowImage;
	//double
	IplImage* flowField;
};
