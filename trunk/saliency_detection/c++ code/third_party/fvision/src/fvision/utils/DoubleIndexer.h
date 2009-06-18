#pragma once

#include <set>
#include <map>
#include <vector>

namespace fvision {

class DoubleIndexer {
public:
	DoubleIndexer();
	DoubleIndexer(const std::set<int>& ids);
	~DoubleIndexer(void);

public:
	///assign a new id from 0 to N
	void initialize(const std::set<int>& ids);

	template<typename Iterator>
	void initialize(Iterator begin, Iterator end) {
		newIdKeyMap.clear();
		originalKeyMap.clear();
		newId = 0;
		mSize = 0;
		Iterator iter = begin;
		for (; iter != end; iter++) {
			int id = *iter;
			addId(id);
			mSize++;
		}
	}

	int size() const { return mSize; }

	int getNewId(int originalId) const ;

	int getOriginalId(int newId) const ;

	void addId(int id);

	void addId(int originalId, int newId);

	void getAllOriginalIds(std::vector<int>& ids) const ;

private:
	std::map<int, int> newIdKeyMap;
	std::map<int, int> originalKeyMap;

	int mSize;

	int newId;
};

}