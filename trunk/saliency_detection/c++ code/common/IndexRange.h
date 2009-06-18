#ifndef INDEX_RANGE_H_
#define INDEX_RANGE_H_

#include <iosfwd>

class IndexRange {
public:
	IndexRange(void);
	IndexRange(int startIndex, int endIndex);
	~IndexRange(void);
public:
	void shiftIndex(int offset) {
		startIndex += offset;
		endIndex += offset;
	}

	int getLength() const {
		return endIndex - startIndex;
	}

public:
	int startIndex;
	int endIndex;
};

std::ostream& operator<< (std::ostream& os, const IndexRange& range);
std::istream& operator>> (std::istream& is, IndexRange& range);

#endif // INDEX_RANGE_H_
