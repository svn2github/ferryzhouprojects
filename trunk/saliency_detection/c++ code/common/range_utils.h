#ifndef RANGE_UTILS_H_
#define RANGE_UTILS_H_

#include "IndexRangeList.h"

int getTotalRangeLength(const IndexRangeList& ranges);

int getRangeOverlap(const IndexRange& range1, const IndexRange& range2);

int getRangeListOverlap(const IndexRangeList& ranges1, const IndexRangeList& ranges2);

#endif // RANGE_UTILS_H_