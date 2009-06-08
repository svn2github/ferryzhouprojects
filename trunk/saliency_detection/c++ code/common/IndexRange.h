#pragma once

#include <iosfwd>

class IndexRange {
public:
	IndexRange(void);
	~IndexRange(void);
public:
	void shiftIndex(int offset) {
		startIndex += offset;
		endIndex += offset;
	}
public:
	int startIndex;
	int endIndex;
};

std::ostream& operator<< (std::ostream& os, const IndexRange& range);
std::istream& operator>> (std::istream& is, IndexRange& range);

