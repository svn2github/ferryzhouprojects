#include "InitialBackgroundAnalizerClusterImpl.h"

#include "Cluster.h"

class PixelFeature : public Feature {
public:
	PixelFeature() {
		color[0] = color[1] = color[2] = 0;
	}

	PixelFeature(const uchar* c) {
		setColor(c);
	}

public:
	double computeDistance(const Feature* rhs) {
		const PixelFeature* frhs = dynamic_cast<const PixelFeature*>(rhs);
		assert (frhs != NULL);
		double sum = 0;
		double diff;
		for (int i = 0; i < 3; i++) {
			diff = (double)color[i] - frhs->color[i];
			sum += diff * diff;
		}
		sum /= 3;
		return sqrt(sum);
	}

	Feature* clone() const {
		return new PixelFeature(this->color);
	}

	void setColor(const uchar* c) {
		color[0] = c[0]; color[1] = c[1]; color[2] = c[2];
	}

public:
	uchar color[3];
};

class PixelCluster : public Cluster {
public:
	PixelCluster() {
		clusterSize = 0;
	}

	PixelCluster(const PixelFeature* feature) {
		this->clusterSize = 1;
		this->meanFeature = *feature;
	}

public:
	static double distanceThreshold;

public:
	bool isIn(const Feature* feature) {
		return meanFeature.computeDistance(feature) <= distanceThreshold;
	}

	void add(const Feature* feature) {
		const PixelFeature* pixelFeature = dynamic_cast<const PixelFeature*>(feature);
		assert (pixelFeature != NULL);
		double alpha = (double)size() / (size() + 1);
		double beta = 1.0 - alpha;
		for (size_t i = 0; i < 3; i++) {
			meanFeature.color[i] = (uchar)(alpha * meanFeature.color[i] + beta * pixelFeature->color[i]);
		}
		++clusterSize;
	}

	Cluster* create(const Feature* feature) {
		return new PixelCluster((const PixelFeature*)feature);
	}

	size_t size() const {
		return clusterSize;
	}

	const Feature* getMean() const { return &meanFeature; }

private:
	size_t clusterSize;
	PixelFeature meanFeature;
};

double PixelCluster::distanceThreshold = 30;

InitialBackgroundAnalizerClusterImpl::InitialBackgroundAnalizerClusterImpl(void) {
	backgroundImage = NULL;
}

InitialBackgroundAnalizerClusterImpl::~InitialBackgroundAnalizerClusterImpl(void)
{
}

void InitialBackgroundAnalizerClusterImpl::addTrainingFrame(const IplImage* image) {
	assert (image != NULL);
	if (clusterManagers.size() == 0) {
		imageWidth = image->width;
		imageHeight = image->height;
		pixelCount = imageWidth * imageHeight;
		clusterManagers.resize(pixelCount);
		for (int i = 0; i < pixelCount; i++) {
			clusterManagers[i] = new ClusterManager(new PixelCluster());
		}
	}
	assert (image->width == imageWidth && imageHeight == imageHeight);
	//update clusters for each pixel
	uchar* data = (uchar*)image->imageData;
	PixelFeature feature;
	int k = 0;
	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {
			feature.setColor(data + i * image->widthStep + j * image->nChannels);
			clusterManagers[k++]->addFeature(&feature);
		}
	}
}

const IplImage* InitialBackgroundAnalizerClusterImpl::getBackgroundImage() {
	assert (clusterManagers.size() != 0);
	if (backgroundImage == NULL) {
		backgroundImage = cvCreateImage(cvSize(imageWidth, imageHeight), 8, 3);
	}
	uchar* data = (uchar*)backgroundImage->imageData;
	int k = 0;
	int widthStep = backgroundImage->widthStep;
	for (int i = 0; i < imageHeight; i++) {
		int offset = i * widthStep;
		for (int j = 0; j < imageWidth; j++) {
			PixelCluster* cluster = (PixelCluster*)(clusterManagers[k++]->getMaxSizeCluster());
			const PixelFeature* feature = (const PixelFeature*)cluster->getMean();
			uchar* pixel = data + offset + j * backgroundImage->nChannels;
			for (int t = 0; t < 3; t++) pixel[t] = feature->color[t];
		}
	}
	return backgroundImage;
}
