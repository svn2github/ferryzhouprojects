#include "geom.h"

#include "cvmat_utils.h"

#include <cassert>

namespace fvision {
	//pt: 3 x 1
	//line: 3 x 1
	double point2LineDistance(const CvMat* pt, const CvMat* line) {
		assertH3(pt);
		assertH3(line);

		double x0 = cvmGet(pt, 0, 0) / cvmGet(pt, 2, 0);
		double y0 = cvmGet(pt, 1, 0) / cvmGet(pt, 2, 0);
		double a = cvmGet(line, 0, 0);
		double b = cvmGet(line, 1, 0);
		double c = cvmGet(line, 2, 0);

		return abs(a * x0 + b * y0 + c) / sqrt(a * a + b * b);
	}

	double point2LineDistance(const CvPoint2D32f& pt, const CvMat* line, int metric /*= 1*/) {
		assert(metric == 1 || metric == 2);

		double a = cvmGet(line, 0, 0);
		double b = cvmGet(line, 1, 0);
		double c = cvmGet(line, 2, 0);

		double dist = abs(a * pt.x + b * pt.y + c) / sqrt(a * a + b * b);
		if (metric == 1) return dist;
		else return dist * dist;
	}
	double pointDistanceSquare(const CvPoint2D32f& p1, const CvPoint2D32f& p2) {
		double dx = p1.x - p2.x;
		double dy = p1.y - p2.y;
		return dx * dx + dy * dy;
	}

	double pointDistance(const CvPoint2D32f& p1, const CvPoint2D32f& p2, int metric) {
		assert(metric == 1 || metric == 2);

		double square = pointDistanceSquare(p1, p2);
		if (metric == 2) return square;
		else return sqrt(square);
	}

	double pointDistance(const CvPoint& p1, const CvPoint& p2) {
		int dx = p1.x - p2.x;
		int dy = p1.y - p2.y;
		return sqrt((double)(dx * dx + dy * dy));			
	}

	CvPoint3D32f scalePoint3D(CvPoint3D32f p3d, double v) {
		return cvPoint3D32f(p3d.x * v, p3d.y * v, p3d.z * v);
	}

	CvMat* getLine(CvPoint2D32f p1, CvPoint2D32f p2) {
		CvMat* M = cvCreateMat(3, 1, CV_64FC1);
		double mh[] = {p1.y - p2.y, p2.x - p1.x, p1.x * p2.y - p1.y * p2.x};
		cvSetData(M, mh, CV_AUTOSTEP);
		return cloneMat(M);
	}

	CvMat* getLine(CvPoint p1, CvPoint p2) {
		return getLine(cvPointTo32f(p1), cvPointTo32f(p2));
	}

	CvMat* getIntersectionPoint(const vector<CvMat*> ps) {
		CvMat* line1 = crossProduct(ps[0], ps[1]);
		CvMat* line2 = crossProduct(ps[2], ps[3]);
		CvMat* p = crossProduct(line1, line2);
		cvReleaseMat(&line1);
		cvReleaseMat(&line2);
		return p;
	}

	CvPoint3D32f normalizePoint3D(const CvPoint3D32f& p) {
		double length = getPointLength(p);
		return cvPoint3D32f(p.x / length, p.y / length, p.z / length);
	}

	CvPoint3D32f subPoint(const CvPoint3D32f& p1, const CvPoint3D32f& p2) {
		return cvPoint3D32f(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
	}

	CvPoint3D32f addPoint(const CvPoint3D32f& p1, const CvPoint3D32f& p2) {
		return cvPoint3D32f(p2.x + p1.x, p2.y + p1.y, p2.z + p1.z);
	}

	double getPointLength(const CvPoint3D32f& p) {
		return sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
	}

	double getPointLength(const CvPoint3D64f& p) {
		return sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
	}

	CvPoint2D32f getCentroid(const vector<CvPoint2D32f>& ps) {

		if (ps.size() == 0) return cvPoint2D32f(0, 0);

		double x_sum = 0;
		double y_sum = 0;
		for (size_t i = 0; i < ps.size(); i++) {
			x_sum += ps[i].x;
			y_sum += ps[i].y;
		}
		return cvPoint2D32f(x_sum / ps.size(), y_sum / ps.size());
	}

	vector<CvPoint2D32f> movePoints(const vector<CvPoint2D32f>& ps, const CvPoint2D32f& t) {
		vector<CvPoint2D32f> nps;
		for (size_t i = 0; i < ps.size(); i++) {
			nps.push_back(ps[i] + t);
		}
		return nps;
	}

	double pointDistance(const CvPoint3D32f& p1, const CvPoint3D32f& p2, int metric) {
		assert(metric == 1 || metric == 2);
		CvPoint3D32f p12 = subPoint(p1, p2);
		double len = getPointLength(p12);
		if (metric == 1) return len;
		else return len * len;
	}

	double pointDistance(const CvPoint3D64f& p1, const CvPoint3D64f& p2, int metric /*= 1*/) {
		assert(metric == 1 || metric == 2);
		CvPoint3D64f p12 = p1 - p2;
		double len = getPointLength(p12);
		if (metric == 1) return len;
		else return len * len;
	}

	double averagePointDistance(const vector<vector<CvPoint2D32f>>& xss1, const vector<vector<CvPoint2D32f>>& xss2) {
		double errorsum = 0;
		for (size_t i = 0; i < xss1.size(); i++) {
			for (size_t j = 0; j < xss1[i].size(); j++) {
				errorsum += pointDistance(xss1[i][j], xss2[i][j]);
			}
		}
		int N = (int)(xss1.size() * xss1[0].size());
		if (N == 0) return 0;
		return errorsum / N;
	}

	double averagePointDistance(const vector<CvPoint2D32f>& xs1, const vector<CvPoint2D32f>& xs2) {
		double errorsum = 0;
		for (size_t i = 0; i < xs1.size(); i++) {
			errorsum += pointDistance(xs1[i], xs2[i]);
		}
		int N = (int)xs1.size();
		if (N == 0) return 0;
		return errorsum / N;
	}

	double averagePointDistanceSquare(const vector<CvPoint2D32f>& xs1, const vector<CvPoint2D32f>& xs2) {
		double errorsum = 0;
		for (size_t i = 0; i < xs1.size(); i++) {
			errorsum += pointDistanceSquare(xs1[i], xs2[i]);
		}
		int N = (int)xs1.size();
		if (N == 0) return 0;
		return errorsum / N;
	}

	double getPointAngle(const CvPoint3D32f& p1, const CvPoint3D32f& p2) {
		double t = p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
		double ratio = abs(t) / (getPointLength(p1) * getPointLength(p2));
		return acos(ratio);
	}

	CvMat* normalize(const vector<CvPoint2D32f>& xs, vector<CvPoint2D32f>& nxs, CvMat* buf) {
		if (buf != NULL) {
			assert(buf->rows == 3 && buf->cols == 3);
		} else {
			buf = cvCreateMat(3, 3, CV_64FC1);
		}
		float mx = 0;
		float my = 0;
		int n = (int)xs.size();
		int i = 0;
		for (i = 0; i < n; i++) {
			mx += xs[i].x;
			my += xs[i].y;
		}
		mx /= n;
		my /= n;
		for (i = 0; i < n; i++) {
			CvPoint2D32f p;
			p.x = xs[i].x - mx;
			p.y = xs[i].y - my;
			nxs.push_back(p);
		}

		float w = 0;
		float d = 0;
		for (i = 0; i < n; i++) {
			d += sqrt(nxs[i].x * nxs[i].x + nxs[i].y * nxs[i].y);
		}
		d /= n;
		w = sqrt(2.0f) / d;
		for (i = 0; i < n; i++) {
			nxs[i].x *= w;
			nxs[i].y *= w;
		}

		//T = [w, 0, -w*mx; 0, w, -w*my; 0, 0, 1]
		CvMat* T = buf;
		cvZero(T);
		cvmSet(T, 0, 0, w);
		cvmSet(T, 1, 1, w);
		cvmSet(T, 0, 2, -w * mx);
		cvmSet(T, 1, 2, -w * my);
		cvmSet(T, 2, 2, 1);

		return T;
	}

	CvPoint operator+ (const CvPoint& p1, const CvPoint& p2) {
		return cvPoint(p1.x + p2.x, p1.y + p2.y);
	}

	CvPoint2D32f operator+ (const CvPoint2D32f& p1, const CvPoint2D32f& p2) {
		return cvPoint2D32f(p1.x + p2.x, p1.y + p2.y);
	}

	CvPoint operator- (const CvPoint& p1, const CvPoint& p2) {
		return cvPoint(p1.x - p2.x, p1.y - p2.y);
	}

	CvPoint2D32f operator- (const CvPoint2D32f& p1, const CvPoint2D32f& p2) {
		return cvPoint2D32f(p1.x - p2.x, p1.y - p2.y);
	}

	CvPoint operator* (const CvPoint& p1, double v) {
		return cvPoint((int)(p1.x * v), (int)(p1.y * v));
	}

	CvPoint2D32f operator* (const CvPoint2D32f& p1, double v) {
		return cvPoint2D32f(p1.x * v, p1.y * v);
	}

	CvPoint operator/ (const CvPoint& p1, double v) {
		return cvPoint((int)(p1.x / v), (int)(p1.y / v));
	}

	CvPoint2D32f operator/ (const CvPoint2D32f& p1, double v) {
		return cvPoint2D32f(p1.x / v, p1.y / v);
	}

	CvSize operator* (const CvSize& size, double v) {
		return cvSize((int)(size.width * v), (int)(size.height * v));
	}

	CvSize operator/ (const CvSize& size, double v) {
		return cvSize(size.width / 2, size.height / 2);
	}

	vector<CvPoint2D32f> operator+ (const vector<CvPoint2D32f>& pts, const CvPoint2D32f& tp) {
		vector<CvPoint2D32f> npts;
		for (size_t i = 0; i < pts.size(); i++) {
			npts.push_back(pts[i] + tp);
		}
		return npts;
	}



	CvRect operator* (const CvRect& rect, double v) {
		return cvRect((int)(rect.x * v), (int)(rect.y * v), (int)(rect.width * v), (int)(rect.height * v));
	}

	CvRect operator/ (const CvRect& rect, double v) {
		return cvRect((int)(rect.x / v), (int)(rect.y / v), (int)(rect.width / v), (int)(rect.height / v));
	}

	CvPoint3D32f operator* (const CvPoint3D32f& p1, double v) {
		return cvPoint3D32f(p1.x * v, p1.y * v, p1.z * v);
	}

	vector<CvPoint3D32f> operator* (const vector<CvPoint3D32f>& ps, double v) {
		vector<CvPoint3D32f> nps;
		for (size_t i = 0; i < ps.size(); i++) {
			nps.push_back(ps[i] * v);
		}
		return nps;
	}

	CvPoint3D32f operator+ (const CvPoint3D32f& p1, const CvPoint3D32f& p2) {
		return cvPoint3D32f(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
	}

	CvPoint3D32f operator- (const CvPoint3D32f& p1, const CvPoint3D32f& p2) {
		return cvPoint3D32f(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
	}

	CvPoint3D32f operator/ (const CvPoint3D32f& p1, double v) {
		return cvPoint3D32f(p1.x / v, p1.y / v, p1.z / v);
	}

	double operator* (const CvPoint3D32f& p1, const CvPoint3D32f& p2) {
		return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
	}

	//========= Operators for CvPoint3D32f ===================>
	CvPoint3D64f operator+ (const CvPoint3D64f& p1, const CvPoint3D64f& p2) {
		return cvPoint3D64f(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
	}

	CvPoint3D64f operator- (const CvPoint3D64f& p1, const CvPoint3D64f& p2) {
		return cvPoint3D64f(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
	}

	CvPoint3D64f operator* (const CvPoint3D64f& p1, double v) {
		return cvPoint3D64f(p1.x * v, p1.y * v, p1.z * v);
	}

	CvPoint3D64f operator/ (const CvPoint3D64f& p1, double v) {
		return cvPoint3D64f(p1.x / v, p1.y / v, p1.z / v);
	}

	CvPoint2D32f rotatePoint(const CvPoint2D32f& p, double theta) {
		return cvPoint2D32f(cos(theta) * p.x - sin(theta) * p.y, sin(theta) * p.x + cos(theta) * p.y);
	}

	CvPoint3D64f rotatePoint(const CvPoint3D64f& p, const CvMat* R) {
		double nx = cvmGet(R, 0, 0) * p.x + cvmGet(R, 0, 1) * p.y + cvmGet(R, 0, 2) * p.z;
		double ny = cvmGet(R, 1, 0) * p.x + cvmGet(R, 1, 1) * p.y + cvmGet(R, 1, 2) * p.z;
		double nz = cvmGet(R, 2, 0) * p.x + cvmGet(R, 2, 1) * p.y + cvmGet(R, 2, 2) * p.z;
		return cvPoint3D64f(nx, ny, nz);
	}

	bool computeLineInBoundary(const CvMat* line, const CvSize& size, CvPoint2D32f& p1, CvPoint2D32f& p2) {
		double a = cvmGet(line, 0, 0);
		double b = cvmGet(line, 1, 0);
		double c = cvmGet(line, 2, 0);

		vector<CvPoint2D32f> boundaryPoints;
		//compute four intersection points:
		double x = 0;
		double y = 0;
		double w = size.width - 1;
		double h = size.height - 1;
		
		//ax + by + c = 0

		//first point: x = 0
		y = -c / b;
		if (y >= 0 && y <= h) {
			boundaryPoints.push_back(cvPoint2D32f(0, y));
		}

		//second point: x = w
		y = (-c - a * w) / b;
		if (y >= 0 && y <= h) {
			boundaryPoints.push_back(cvPoint2D32f(w, y));
		}

		//third point: y = 0
		x = -c / a;
		if (x >= 0 && x <= w) {
			boundaryPoints.push_back(cvPoint2D32f(x, 0));
		}

		//fourth point: y = h
		x = (-c - b * h) / a;
		if (x >= 0 && x <= w) {
			boundaryPoints.push_back(cvPoint2D32f(x, h));
		}

		//assert(boundaryPoints.size() == 0 || boundaryPoints.size() == 2 );

		if (boundaryPoints.size() < 2) return false;

		p1 = boundaryPoints[0];
		p2 = boundaryPoints[1];
		return true;
	}
}