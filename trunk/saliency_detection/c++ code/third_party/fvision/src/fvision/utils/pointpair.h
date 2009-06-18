#pragma once

#include <cv.h>

#include <utility>
#include <vector>

namespace fvision {

typedef std::pair<CvPoint2D32f, CvPoint2D32f> PointPair;
typedef std::vector<PointPair> PointPairs;

typedef std::pair<CvPoint, CvPoint> PointIntPair;
typedef std::vector<PointIntPair> PointIntPairs;


std::vector<CvPoint2D32f> getFirstPoints(const PointPairs& pps);
void decomposePointPairs(const PointPairs& pps, std::vector<CvPoint2D32f>& x1s, std::vector<CvPoint2D32f>& x2s);
void composePointPairs(PointPairs& pps, const std::vector<CvPoint2D32f>& x1s, const std::vector<CvPoint2D32f>& x2s);

std::vector<CvPoint2D32f> getDisparities(const PointPairs& pps);

PointPairs getSubSet(const PointPairs& pps, const std::vector<int>& indices);

template <typename UnaryOperator>
void transformPointPairs(PointPairs& pps, const UnaryOperator& op) {
	PointPairs::iterator iter = pps.begin();
	for (; iter != pps.end(); iter++) {
		iter->first = op(iter->first);
		iter->second = op(iter->second);
	}
}

template <typename UnaryOperator>
void copyTransformPointPairs(const PointPairs& src, PointPairs& dst, const UnaryOperator& op) {
	assert(src.size() == dst.size());
	PointPairs::const_iterator iter = src.begin();
	PointPairs::iterator dstIter = dst.begin();
	for (; iter != src.end(); iter++, dstIter++) {
		dstIter->first = op(iter->first);
		dstIter->second = op(iter->second);
	}
}


std::ostream& operator<<(std::ostream& os, const PointPairs& pps);
std::istream& operator>>(std::istream& is, PointPairs& pps);

}
