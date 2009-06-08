#pragma once

#include "ClusterManager.h"
#include "ImageBlockFeature.h"

#include <cv.h>
#include <vector>

class ImageBlockFeatureExtractor {
public:
	virtual ~ImageBlockFeatureExtractor() {}
	virtual ImageBlockFeature* extract(const IplImage* frame, size_t xCoord, size_t yCoord, size_t id) = 0;
};


//input an image sequence
//do background statistics, assume the scene is static
//for new images or original images
//do background foreground separation
//
class BackgroundAnalizer {
public:
	BackgroundAnalizer(size_t blockSize = 8);
	~BackgroundAnalizer(void);
public:
	void addFrame(const IplImage* frame);
	void getBackgroundImage(IplImage* frame);
	size_t getGridSize() const ;

	//get background
	std::vector<ImageBlockFeature*> getBackgroundFeatures();
	void renderImageWithBackgroundFeatures(IplImage* frame, const std::vector<ImageBlockFeature*>& features) const;

	//classify a target image
	void testFrame(const IplImage* src);
	const double* getRawDistances() const { return distances; }
	const IplImage* getRawDistanceImage() const { return rawDistanceImage; }
	const IplImage* getForegroundBlockImage() const { return foregroundBlockImage; }

private:
	void initTestFrameBuffers();

	//gridResults should have size equal to grid size
	void calculateBlockDistanceWithBackgroundFeatures(const IplImage* src, double* distances);

	void compteRawDistanceImage(const double* distances, IplImage* distanceImage) const;

	void computeForegroundBlockImage(const double* distances, IplImage* foregroundImage) const;

private:
	//setup xCoords, yCoords, patchClusterManagers
	void initializeWithImageSize(const CvSize& imageSize);

	void fillBlock(IplImage* frame, size_t xi, size_t yi, const CvScalar& color) const ;

private:
	//we ignore the boundary blocks
	//thus we can ensure last coord + blocksize is smaller than the boundary
	size_t blockSize;
	std::vector<size_t> xCoords;
	std::vector<size_t> yCoords;
	size_t xNum;
	size_t yNum;
	size_t totalNum;

	CvSize imageSize;

	bool trainingStarted;
	bool testingStarted;

	std::vector<ClusterManager*> patchClusterManagers;
	ImageBlockFeatureExtractor* featureExtractor;

	std::vector<ImageBlockFeature*> features;

	Feature* backgroundFeatures;

	//buffer for testing image
	double* distances;
	IplImage* rawDistanceImage;
	IplImage* foregroundBlockImage;
};
