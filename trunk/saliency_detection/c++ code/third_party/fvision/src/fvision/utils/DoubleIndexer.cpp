#include "DoubleIndexer.h"

#include <cassert>

using namespace std;

namespace fvision {

DoubleIndexer::DoubleIndexer(const std::set<int>& ids) {
	initialize(ids);
}

DoubleIndexer::DoubleIndexer() {
	newId = 0;
}
DoubleIndexer::~DoubleIndexer(void)
{
}

void DoubleIndexer::initialize(const std::set<int>& ids) {
	initialize(ids.begin(), ids.end());
}

int DoubleIndexer::getNewId(int originalId) const {
	map<int, int>::const_iterator iter = originalKeyMap.find(originalId);
	assert(iter != originalKeyMap.end());
	return iter->second;
}

int DoubleIndexer::getOriginalId(int newId) const {
	map<int, int>::const_iterator iter = newIdKeyMap.find(newId);
	assert(iter != newIdKeyMap.end());
	return iter->second;
}

void DoubleIndexer::addId(int id) {
	newIdKeyMap[newId] = id;
	originalKeyMap[id] = newId;
	newId++;	
}

void DoubleIndexer::addId(int originalId, int newId) {
	assert(originalKeyMap.find(originalId) == originalKeyMap.end() && newIdKeyMap.find(newId) == newIdKeyMap.end());
	originalKeyMap[originalId] = newId;
	newIdKeyMap[newId] = originalId;
	if (newId >= this->newId) this->newId = newId + 1;
}

void DoubleIndexer::getAllOriginalIds(std::vector<int>& ids) const {
	ids.resize(newId);
	for (int i = 0; i < newId; i++) {
		ids[i] = getOriginalId(i);
	}
}
}