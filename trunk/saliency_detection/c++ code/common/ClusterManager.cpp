#include "ClusterManager.h"

#include <cassert>
#include <algorithm>

ClusterManager::ClusterManager(Cluster* creator) {
	this->creator = creator;
}

ClusterManager::~ClusterManager(void) {
	delete creator;
	for (size_t i = 0; i < clusters.size(); i++) {
		delete clusters[i];
		clusters[i] = NULL;
	}
}

void ClusterManager::addFeature(const Feature* feature) {
	for (size_t i = 0; i < clusters.size(); i++) {
		if (clusters[i]->isIn(feature)) { //in cluster
			clusters[i]->add(feature);
			return;
		}
	}
	clusters.push_back(creator->create(feature));
}

class ClusterCmp {
public:
	bool operator() (Cluster* c1, Cluster* c2) {
		return c1->size() < c2->size();
	}
};

Cluster* ClusterManager::getMaxSizeCluster() const {
	return *max_element(clusters.begin(), clusters.end(), ClusterCmp());
}