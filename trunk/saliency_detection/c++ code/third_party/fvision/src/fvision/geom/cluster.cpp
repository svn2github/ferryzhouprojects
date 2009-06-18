#include "cluster.h"

#include <fvision/utils.h>

#include <algorithm>

namespace fvision {

bool compareClusterBySize(Cluster* c1, Cluster* c2) {
	return c1->size() > c2->size();
}

bool PointCluster::isIn(const CvPoint2D32f& p, double maxDistance) const {
	return pointDistance(avg, p) <= maxDistance;
}

void PointCluster::add(int index, const CvPoint2D32f& p) {
	avg = (avg * size() + p) / (size() + 1);
	indices.push_back(index);
}

vector<Cluster*> PointClusterer::compute(const vector<CvPoint2D32f>& ps) {
	vector<Cluster*> clusters;
	//iterate once
	for (int i = 0; i < (int)ps.size(); i++) {
		bool newc = true;
		for (int j = 0; j < (int)clusters.size(); j++) {
			PointCluster* pcluster = (PointCluster*)clusters[j];
			if (pcluster->isIn(ps[i], maxDistance)) { //in cluster
				pcluster->add(i, ps[i]);
				newc = false;
				break;
			}
		}
		if (newc) { //create a new cluster
			clusters.push_back(new PointCluster(i, ps[i]));
		}
	}
	sort(clusters.begin(), clusters.end(), compareClusterBySize);
	return clusters;
}

vector<Cluster*> MotionClusterer::compute(const PointPairs& pps) {

	vector<CvPoint2D32f> ds = getDisparities(pps);
	PointClusterer pc(maxDistance);
	return pc.compute(ds);
}
/*
vector<Cluster*> HomographyClusterer::compute(const PointPairs& pps) {
	vector<Cluster*> clusters;
	//iterative pick a point from non cluster and then take several closest points and compute homography and expand
	vector<int> freshIndices;
	int iternum = 0;
	while ((++i) < 10) {
		HomographyCluster* hc = iterateOnce(freshIndices, pps);
		clusters.push_back(hc);
		freshIndices = excludeClusterIndices(hc, freshIndices);
	}
	return clusters;
}

HomographyCluster* iterateOnce(const vector<int>& freshIndices, const PointPairs& pps) {
	int r = rani(freshIndices.size());
	int ri = freshIndices[r];
	vector<int> points = pickClosestPoints(ri, freshIndices, pps, homographySampleNum);
	return getCluster(points, freshIndices, pps);
}

vector<int> pickClosestPoints(int index, const vector<int>& freshIndices, const PointPairs& pps, int num) {

}
*/
vector<int> SampleClusterBasedFilter::filter(const vector<Cluster*>& clusters) {
	vector<int> nindices;
	for (int i = 0; i < (int)clusters.size(); i++) {
		int size = clusters[i]->size();
		if (size < MIN_CLUSTER_SIZE) break; //outlier
		if (sampleNum < 0) {
			nindices.insert(nindices.begin(), clusters[i]->indices.begin(), clusters[i]->indices.end());
		} else {
			vector<int> sampleInts = ranis(size - 1, sampleNum);
			for (int j = 0; j < (int)sampleInts.size(); j++) {
				nindices.push_back(clusters[i]->indices[sampleInts[j]]);
			}
		}

/*		if (size < 5) {  //small cluster, add all
			nindices.insert(nindices.begin(), clusters[i].indices.begin(), clusters[i].indices.end());
		} else {
			int num = 5;
			if (size > 10) num = 6;
			vector<int> rands = ranis(size - 1, num);
			vector<int> indices;
			for (int j = 0; j < rands.size(); j++) {
				indices.push_back(clusters[i].indices[rands[j]]);
			}
			nindices.insert(nindices.begin(), indices.begin(), indices.end());
		}
*/	}
	return nindices;
}
}