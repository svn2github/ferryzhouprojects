#pragma once

#include "Cluster.h"

#include <vector>

class ImageBlockCluster : public Cluster {
private:
	ImageBlockCluster() {}
	ImageBlockCluster(const Feature* feature);
public:
	static ImageBlockCluster* createInstance() { return new ImageBlockCluster(); }

	~ImageBlockCluster(void);
public:
	bool isIn(const Feature* feature);
	void add(const Feature* feature);
	Cluster* create(const Feature* feature);
	size_t size() const;

	const Feature* getMean() const;
	double getMeanDistance() const;

private:
	std::vector<size_t> ids;
	Feature* mean;
	double meanDistance;
public:
	static double threshold;
};
