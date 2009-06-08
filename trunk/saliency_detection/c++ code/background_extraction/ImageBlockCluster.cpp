#include "ImageBlockCluster.h"

#include "ImageBlockFeature.h"

double ImageBlockCluster::threshold = 20.0;

ImageBlockCluster::ImageBlockCluster(const Feature* feature) {
	this->mean = feature->clone();
	const ImageBlockFeature* blockFeature = dynamic_cast<const ImageBlockFeature*>(feature);
	ids.push_back(blockFeature->id);
	meanDistance = 0;
}

ImageBlockCluster::~ImageBlockCluster(void) {
	delete mean;
}

bool ImageBlockCluster::isIn(const Feature* feature) {
	return mean->computeDistance(feature) <= threshold;
}

Cluster* ImageBlockCluster::create(const Feature* feature) {
	ids.push_back(((ImageBlockFeature*)feature)->id);
	return new ImageBlockCluster(feature);	
}

size_t ImageBlockCluster::size() const {
	return ids.size();
}

void ImageBlockCluster::add(const Feature* feature) {
	ImageBlockFeature* blockFeature = (ImageBlockFeature*)feature;
	ImageBlockFeature* blockMean = (ImageBlockFeature*)mean;
	double alpha = (double)ids.size() / (ids.size() + 1);
	double beta = 1.0 - alpha;
	for (size_t i = 0; i < blockMean->length; i++) {
		blockMean->values[i] = alpha * blockMean->values[i] + beta * blockFeature->values[i];
	}
	ids.push_back(((ImageBlockFeature*)feature)->id);
}

const Feature* ImageBlockCluster::getMean() const {
	return mean;
}

double ImageBlockCluster::getMeanDistance() const {
	return meanDistance;
}