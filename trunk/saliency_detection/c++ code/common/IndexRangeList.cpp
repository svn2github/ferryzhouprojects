#include "IndexRangeList.h"

IndexRangeList::IndexRangeList(void)
{
}

IndexRangeList::~IndexRangeList(void)
{
}

void IndexRangeList::addIndexRange(const IndexRange& range) {
	ranges.push_back(range);
}