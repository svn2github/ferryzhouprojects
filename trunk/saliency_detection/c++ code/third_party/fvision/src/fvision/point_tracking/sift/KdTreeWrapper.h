#pragma once

#include <vector>

extern "C" {
#include "imgfeatures.h"
#include "kdtree.h"
}

namespace fvision {

// a wrapper c++ class for kdtree
class KdTree {
private:
	struct kd_node* kd_root;
	// the maximum number of keypoint NN candidates to check during BBF search, default is 200
	int KDTREE_BBF_MAX_NN_CHKS;

public:
	KdTree(struct feature* features, int n);
	~KdTree(void);

public:
	std::vector<feature*> getKNearestNeighbors(struct feature* feat, int k);

	// set the maximum number of keypoint NN candidates to check during BBF search, default is 200
	void setMaxNnCandidate(int n) { this->KDTREE_BBF_MAX_NN_CHKS = n; }

};

}