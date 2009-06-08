#pragma once

#include "Feature.h"
#include "Cluster.h"

#include <vector>

//add a feature
//
class ClusterManager {
public:
	ClusterManager(Cluster* creator);
	~ClusterManager(void);
public:
	void addFeature(const Feature* feature);
	Cluster* getMaxSizeCluster() const;

private:
	std::vector<Cluster*> clusters;
	Cluster* creator;
};
