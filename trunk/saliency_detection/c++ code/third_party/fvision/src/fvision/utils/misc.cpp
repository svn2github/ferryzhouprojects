#include "misc.h"

#include "math.h"

#include "random_utils.h"

#include <algorithm>

using namespace std;

namespace fvision {

vector<CvPoint> cvPointsFrom32fs(const vector<CvPoint2D32f>& pts) {
	vector<CvPoint> ps;
	for (int i = 0; i < (int)pts.size(); i++) {
		ps.push_back(cvPointFrom32f(pts[i]));
	}
	return ps;
}

vector<CvPoint2D32f> cvPointsTo32fs(const vector<CvPoint>& pts) {
	vector<CvPoint2D32f> ps;
	for (int i = 0; i < (int)pts.size(); i++) {
		ps.push_back(cvPointTo32f(pts[i]));
	}
	return ps;
}

CvPoint3D32f cvPoint3D32fFrom64f(const CvPoint3D64f& p) {
	return cvPoint3D32f(p.x, p.y, p.z);
}

CvPoint3D64f cvPoint3D32fTo64f(const CvPoint3D32f& p) {
	return cvPoint3D64f(p.x, p.y, p.z);
}

vector<CvPoint3D32f> cvPoint3D32fsFrom64fs(const vector<CvPoint3D64f>& pts) {
	vector<CvPoint3D32f> npts(pts.size());
	transform(pts.begin(), pts.end(), npts.begin(), cvPoint3D32fFrom64f);
	return npts;
}

vector<CvPoint3D64f> cvPoint3D32fsTo64fs(const vector<CvPoint3D32f>& pts) {
	vector<CvPoint3D64f> npts(pts.size());
	transform(pts.begin(), pts.end(), npts.begin(), cvPoint3D32fTo64f);
	return npts;
}

void randomlySampleNPointPairs(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s, 
	vector<CvPoint2D32f>& sx1s, vector<CvPoint2D32f>& sx2s, int n) {

	vector<int> is = ranis((int)x1s.size(), n);
	for (int i = 0; i < n; i++) {
		int index = is[i];
		sx1s.push_back(x1s[index]);
		sx2s.push_back(x2s[index]);
	}
}

CvPoint2D32f addNoiseToPoint(float standardDeviation, const CvPoint2D32f& pt) {
	return cvPoint2D32f(pt.x + gaussianRand(0, standardDeviation), pt.y + gaussianRand(0, standardDeviation));
}

vector<CvPoint2D32f> addNoiseToPoints(float standardDeviation, vector<CvPoint2D32f>& pts) {
	vector<CvPoint2D32f> npts;
	for (int i = 0; i < (int)pts.size(); i++) {
		npts.push_back(cvPoint2D32f(pts[i].x + gaussianRand(0, standardDeviation), pts[i].y + gaussianRand(0, standardDeviation)));
	}
	return npts;
}

std::vector<double> addNoiseToPoints(float standardDeviation, std::vector<double>& pts) {
	vector<double> npts(pts.size());
	for (int i = 0; i < (int)pts.size(); i++) {
		npts[i] = (double)gaussianRand((float)pts[i], standardDeviation);
	}
	return npts;
}

vector<int> intersection(const vector<int>& v1, const vector<int>& v2) {
	vector<int> sv1 = v1;
	sort(sv1.begin(), sv1.end());
	vector<int> sv2 = v2;
	sort(sv2.begin(), sv2.end());
	vector<int> nv(v1.size() + v2.size());
	vector<int>::iterator iter = set_intersection(sv1.begin(), sv1.end(), sv2.begin(), sv2.end(), nv.begin());
	nv.resize(iter - nv.begin());
	return nv;
}

std::vector<int> intersection(const std::set<int>& v1, const std::set<int>& v2) {
	vector<int> nv(v1.size() + v2.size());
	vector<int>::iterator iter = set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), nv.begin());
	nv.resize(iter - nv.begin());
	return nv;
}

vector<int> subtraction(const vector<int>& v1, const vector<int>& v2) {
	vector<int> nv;
	for (size_t i = 0; i < v1.size(); i++) {
		if (find(v2.begin(), v2.end(), v1[i]) == v2.end()) nv.push_back(v1[i]);
	}
	return nv;
}

void getInlierOutlierIndices(const vector<double>& errors, double threshold, vector<int>& inlierIndices, vector<int>& outlierIndices) {
	inlierIndices.clear();
	outlierIndices.clear();
	for (int i = 0; i < (int)errors.size(); i++) {
		if (errors[i] <= threshold) inlierIndices.push_back(i);
		else outlierIndices.push_back(i);
	}
}

CvPoint2D32f cvPoint2D32fFrom64f(const CvPoint2D64f& p) {
	return cvPoint2D32f((float)p.x, (float)p.y);
}

std::vector<double> getDisparities(const std::vector<CvPoint2D32f>& x1s, const std::vector<CvPoint2D32f>& x2s) {
	assert(x1s.size() == x2s.size());
	vector<double> ds(x1s.size());
	for (size_t i = 0; i < x1s.size(); i++) {
		ds[i] = x2s[i].x - x1s[i].x;
	}
	return ds;
}

std::vector<int> createIntVector(int from, int to, int step /*= 1*/) {
	assert (step * (to - from) > 0); //same direction
	vector<int> vs;
	for (int i = from; i < to; i += step) {
		vs.push_back(i);
	}
	return vs;
}
}
