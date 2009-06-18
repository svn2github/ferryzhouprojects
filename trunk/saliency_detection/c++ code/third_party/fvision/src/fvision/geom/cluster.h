#pragma once

#include <fvision/utils/pointpair.h>

#include <cv.h>

//cluster, compute multiple models from a set
//sampling step (repetitive or exclusive)
//expansion step
//

namespace fvision {

//a cluster is a set of indices
class Cluster {
public:
	int size() const { return (int)indices.size(); }
	std::vector<int> indices;
};

bool compareClusterBySize(Cluster* c1, Cluster* c2);

class PointCluster : public Cluster {
public:
	PointCluster(int i, const CvPoint2D32f& p) {
		add(i, p);
	}

public:
	void add(int index, const CvPoint2D32f& p);
	bool isIn(const CvPoint2D32f& p, double maxDistance) const ;

public:
	CvPoint2D32f avg;
};

//clustering based on motion similarity
typedef PointCluster MotionCluster;
//class MotionCluster : public PointCluster{
//};

//clustering based on homography
class HomographyCluster: public Cluster {
public:
	CvMat* H;
};

class PointClusterer {
public:
	PointClusterer(double maxDistance) {
		this->maxDistance = maxDistance;
	}
public:
	std::vector<Cluster*> compute(const std::vector<CvPoint2D32f>& ps);
private:
	double maxDistance;
};

class MotionClusterer {
public:
	MotionClusterer(double maxDistance) {
		this->maxDistance = maxDistance;
	}
public:
	std::vector<Cluster*> compute(const PointPairs& pps);
private:
	double maxDistance;
};

class HomographyClusterer {
public:
	HomographyClusterer(int homographySampleNum) {
		this->homographySampleNum = homographySampleNum;
	}

public:
	std::vector<Cluster*> compute(const PointPairs& pps);

private:
	//find a cluster in fresh points
	HomographyCluster* iterateOnce(const std::vector<int>& freshIndices, const PointPairs& pps);
	//given a point, find several closest points
	std::vector<int> pickClosestPoints(int index, const std::vector<int>& freshIndices, const PointPairs& pps, int num);
	//iteratively expand a cluster from initial points using homography
	HomographyCluster* getCluster(const std::vector<int>& points, const std::vector<int>& freshIndices, const PointPairs& pps);

private:
	int homographySampleNum;
};

//sample a subset from each cluster
class ClusterBasedFilter {
public:
	virtual std::vector<int> filter(const std::vector<Cluster*>& clusters) = 0;
};

//for each cluster, sample a subset
class SampleClusterBasedFilter {
public:
	//if sampleNum == -1, include the whole cluster
	SampleClusterBasedFilter(int sampleNum, int minClusterSize = 3) {
		this->sampleNum = sampleNum;
		this->MIN_CLUSTER_SIZE = minClusterSize;
	}
public:
	std::vector<int> filter(const std::vector<Cluster*>& clusters);
private:
	int MIN_CLUSTER_SIZE; //threshold of outlier clusters
	int sampleNum;
};

}