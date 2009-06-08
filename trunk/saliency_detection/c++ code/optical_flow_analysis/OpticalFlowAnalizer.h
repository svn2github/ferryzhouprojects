#pragma once

//for each frame, input a set of optical flow particles
//    for each particle, assign it to a grid. For each grid, add the particle to its clusters.
//
class OpticalFlowAnalizer {
public:
	OpticalFlowAnalizer(void);
	~OpticalFlowAnalizer(void);
};
