#include "pointpair.h"

#include "io.h"
#include "geom.h"

#include <cassert>

using namespace std;

namespace fvision {

vector<CvPoint2D32f> getFirstPoints(const PointPairs& pps) {
	vector<CvPoint2D32f> ps;
	for (size_t i = 0; i < pps.size(); i++) {
		ps.push_back(pps[i].first);
	}
	return ps;
}

void decomposePointPairs(const PointPairs& pps, vector<CvPoint2D32f>& x1s, vector<CvPoint2D32f>& x2s) {
	for (size_t i = 0; i < pps.size(); i++) {
		x1s.push_back(pps[i].first);
		x2s.push_back(pps[i].second);
	}
}

void composePointPairs(PointPairs& pps, const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s) {
	assert(x1s.size() == x2s.size());
	for (size_t i = 0; i < x1s.size(); i++) {
		pps.push_back(PointPair(x1s[i], x2s[i]));
	}
}

vector<CvPoint2D32f> getDisparities(const PointPairs& pps) {
	vector<CvPoint2D32f> ds;
	for (size_t i = 0; i < pps.size(); i++) {
		ds.push_back(pps[i].second - pps[i].first);
	}
	return ds;
}

PointPairs getSubSet(const PointPairs& pps, const vector<int>& indices) {
	PointPairs npps;
	for (size_t i = 0; i < indices.size(); i++) {
		npps.push_back(pps[indices[i]]);
	}
	return npps;
}

ostream& operator<<(ostream& os, const PointPairs& pps) {
	vector<CvPoint2D32f> x1s, x2s;
	decomposePointPairs(pps, x1s, x2s);
	os<<x1s<<x2s;
	return os;
}

istream& operator>>(istream& is, PointPairs& pps) {
	pps.clear();
	vector<CvPoint2D32f> x1s, x2s;
	is>>x1s>>x2s;
	composePointPairs(pps, x1s, x2s);
	return is;
}



}