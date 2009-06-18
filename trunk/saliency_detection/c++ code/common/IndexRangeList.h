#ifndef INDEX_RANGE_LIST_H_
#define INDEX_RANGE_LIST_H_

#include "IndexRange.h"
#include <vector>

//the ranges should be sorted
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
	
	void addIndexRange(const IndexRange& range) ;
	void addIndexRange(int startIndex, int endIndex);

private:
	IndexRanges ranges;
};

//write [startIndex endIndex] for each line
std::ostream& operator<< (std::ostream& os, const IndexRangeList& ranges);
//for each line, input two numbers, startIndex and endIndex respectively
std::istream& operator>> (std::istream& is, IndexRangeList& ranges);

#endif // INDEX_RANGE_LIST_H_
