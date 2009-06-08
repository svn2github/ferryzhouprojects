#pragma once

#include "IndexRange.h"
#include <vector>

class IndexRangeList {
public:
	IndexRangeList(void);
	~IndexRangeList(void);

	//use default copy constructors and assignements

	typedef std::vector<IndexRange> IndexRanges;
	typedef IndexRanges::iterator Iterator;
	typedef IndexRanges::const_iterator ConstIterator;

public:
	Iterator begin() { return ranges.begin(); }
	ConstIterator begin() const { return ranges.begin(); }
	Iterator end() { return ranges.end(); }
	ConstIterator end() const { return ranges.end(); }
	
	void addIndexRange(const IndexRange& range);

private:
	IndexRanges ranges;
};
