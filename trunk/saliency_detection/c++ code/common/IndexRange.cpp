#include "IndexRange.h"

#include <iostream>

IndexRange::IndexRange(void)
{
}

IndexRange::~IndexRange(void)
{
}

std::ostream& operator<<(std::ostream& os, const IndexRange& range) {
	os<<range.startIndex<<" "<<range.endIndex;	
	return os;
}

std::istream& operator>>(std::istream& is, IndexRange& range) {
	is>>range.startIndex>>range.endIndex;
	return is;
}