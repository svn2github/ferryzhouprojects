#include "range_utils.h"

#include <algorithm>

using namespace std;

int getTotalRangeLength(const IndexRangeList& ranges) {
	IndexRangeList::ConstIterator iter = ranges.begin();
	int sum = 0;
	for (; iter != ranges.end(); iter++) {
		sum += iter->getLength();
	}
	return sum;
}

int getRangeOverlap(const IndexRange& range1, const IndexRange& range2) {
	if (range1.endIndex <= range2.startIndex) return 0;
	else if (range2.endIndex <= range1.startIndex) return 0;
	else {
		int maxStart = max(range1.startIndex, range2.startIndex);
		int minEnd = min(range1.endIndex, range2.endIndex);
		return minEnd - maxStart;
	}
}

int getRangeListOverlap(const IndexRangeList& ranges1, const IndexRangeList& ranges2) {
	int overlapNum = 0;
	IndexRangeList::ConstIterator iter1 = ranges1.begin();
	IndexRangeList::ConstIterator iter2 = ranges2.begin();
	while (iter1 != ranges1.end() && iter2 != ranges2.end()) {
		if (iter1->endIndex <= iter2->startIndex) iter1++;
		else if (iter2->endIndex <= iter1->startIndex) iter2++;
		else {
			overlapNum += getRangeOverlap(*iter1, *iter2);
			if (iter1->endIndex <= iter2->endIndex) iter1++;
			else iter2++;
		}
	}
	return overlapNum;
}
