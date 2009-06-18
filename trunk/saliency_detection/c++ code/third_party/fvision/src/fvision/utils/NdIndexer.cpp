#include "NdIndexer.h"

#include <cassert>

using namespace std;

namespace fvision {

NdIndexer::NdIndexer(const std::vector<int>& dimLengths) {
	assert (dimLengths.size() != 0);
	this->dimLengths = dimLengths;
	cumDimLengths.resize(dimLengths.size());
	int t = 1;
	for (int i = (int)dimLengths.size() - 1; i >= 0 ; i--) {
		cumDimLengths[i] = t;
		t *= dimLengths[i];
	}
}

NdIndexer::~NdIndexer(void) {
}

int NdIndexer::getArrayIndex(const std::vector<int>& tensorIndices) {
	assert (isTensorIndicesValid(tensorIndices));
	int a = 0;
	for (size_t i = 0; i < cumDimLengths.size(); i++) {
		a += tensorIndices[i] * cumDimLengths[i];
	}
	return a;
}

bool NdIndexer::isTensorIndicesValid(const std::vector<int>& tensorIndices) {
	if (tensorIndices.size() != dimLengths.size()) return false;
	for (size_t i = 0; i < dimLengths.size(); i++) {
		if (tensorIndices[i] < 0 || tensorIndices[i] >= dimLengths[i]) return false;
	}
	return true;
}

std::vector<int> NdIndexer::getTensorIndices(int arrayIndex) {
	int t = arrayIndex;
	vector<int> indices(dimLengths.size());
	for (size_t i = 0; i < dimLengths.size(); i++) {
		indices[i] = t / cumDimLengths[i];
		t = t % cumDimLengths[i];
	}
	return indices;
}

}