#pragma once

#include "InitialBackgroundAnalizer.h"

#include "ClusterManager.h"

#include <vector>

//for each pixel, do clustering
//choose the mean value of the biggest cluster as background
class InitialBackgroundAnalizerClusterImpl : public InitialBackgroundAnalizer {
public:
	InitialBackgroundAnalizerClusterImpl(void);
	~InitialBackgroundAnalizerClusterImpl(void);

public:
	void addTrainingFrame(const IplImage* image);

	const IplImage* getBackgroundImage();

private:
	IplImage* backgroundImage;
	int imageWidth;
	int imageHeight;
	int pixelCount;
	std::vector<ClusterManager*> clusterManagers;	
};
