#include "KdTreeWrapper.h"

namespace fvision {

KdTree::KdTree(struct feature* features, int n) {
	kd_root = kdtree_build(features, n);
	KDTREE_BBF_MAX_NN_CHKS = 200;
}

KdTree::~KdTree(void) {
	kdtree_release(kd_root);
}

std::vector<feature*> KdTree::getKNearestNeighbors(struct feature* feat, int k) {
	struct feature** nearestNeighbors;
	int numOfNeighborsFound = kdtree_bbf_knn(kd_root, feat, k, &nearestNeighbors, KDTREE_BBF_MAX_NN_CHKS);
	std::vector<feature*> neighbors(numOfNeighborsFound);
	for (int i = 0; i < numOfNeighborsFound; i++) {
		neighbors[i] = nearestNeighbors[i];
	}
	return neighbors;
}

}