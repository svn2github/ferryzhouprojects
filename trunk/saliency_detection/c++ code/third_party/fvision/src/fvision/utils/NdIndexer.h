#pragma once

#include <vector>

namespace fvision {

/// convert index between single array and multiple dimension tensor
class NdIndexer {
public:
	NdIndexer(const std::vector<int>& dimLengths);
	~NdIndexer(void);

public:
	/// for n1 x n2 matrix, index (i, j) -> i * n2 + j
	/// for n1 x n2 x n3 tensor, index (i, j, k) -> i * (n2 * n3) + j * n3 + k
	int getArrayIndex(const std::vector<int>& tensorIndices);

	/// (i, j, k) -> p
	/// t = p
	/// i = t / n2 * n3
	/// t = t % i
	/// j = t / n3
	/// t = t % n3
	/// k = t
	std::vector<int> getTensorIndices(int arrayIndex);

	bool isTensorIndicesValid(const std::vector<int>& tensorIndices);

	int getDimCount() const { return (int)dimLengths.size(); }

private:
	std::vector<int> dimLengths;
	std::vector<int> cumDimLengths;
};

}