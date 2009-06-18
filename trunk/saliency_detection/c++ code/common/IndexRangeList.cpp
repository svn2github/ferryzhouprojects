#include "IndexRangeList.h"

#include <iostream>
#include <sstream>

using namespace std;

IndexRangeList::IndexRangeList(void)
{
}

IndexRangeList::~IndexRangeList(void)
{
}

void IndexRangeList::addIndexRange(const IndexRange& range) {
	ranges.push_back(range);
}

void IndexRangeList::addIndexRange(int startIndex, int endIndex) {
	addIndexRange(IndexRange(startIndex, endIndex));
}

std::ostream& operator<<(std::ostream& os, const IndexRangeList& ranges) {
	IndexRangeList::ConstIterator iter = ranges.begin();
	for (; iter != ranges.end(); iter++) {
		os<<iter->startIndex<<" "<<iter->endIndex<<endl;
	}
	return os;
}

std::istream& operator>>(std::istream& is, IndexRangeList& ranges) {
	char line[500];
	while (true) {
		IndexRange range;
		is.getline(line, 500);
		istringstream iss(line);
		if (strlen(line) != 0) {
			iss>>range.startIndex>>range.endIndex;
			ranges.addIndexRange(range);
		} else break;
	}
	return is;
}