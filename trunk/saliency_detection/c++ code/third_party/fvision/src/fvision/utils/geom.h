#pragma once

#include <cv.h>

#include <vector>

using std::vector;

/**
 * geometry utils
 * mainly points and lines utils
 *
 */
namespace fvision {
	//pt: 3 x 1
	//line: 3 x 1
	double point2LineDistance(const CvMat* pt, const CvMat* line);

	double point2LineDistance(const CvPoint2D32f& pt, const CvMat* line, int metric = 1);

	double pointDistanceSquare(const CvPoint2D32f& p1, const CvPoint2D32f& p2);

	double pointDistance(const CvPoint& p1, const CvPoint& p2);

	/** compute 2d point distance. 
	 * ||p1 - p2||k, k = 1, 2
	 * if k == 1, return absolute distance
	 * if k == 2, return distance square
	 */
	double pointDistance(const CvPoint2D32f& p1, const CvPoint2D32f& p2, int metric = 1);

	/** compute 3D point distance. 
	 * ||p1 - p2||k, k = 1, 2
	 * if k == 1, return absolute distance
	 * if k == 2, return distance square
	 */
	double pointDistance(const CvPoint3D32f& p1, const CvPoint3D32f& p2, int metric = 1);
	double pointDistance(const CvPoint3D64f& p1, const CvPoint3D64f& p2, int metric = 1);

	double averagePointDistance(const vector<vector<CvPoint2D32f>>& xss1, const vector<vector<CvPoint2D32f>>& xss2);
	double averagePointDistance(const vector<CvPoint2D32f>& xs1, const vector<CvPoint2D32f>& xs2);
	double averagePointDistanceSquare(const vector<CvPoint2D32f>& xs1, const vector<CvPoint2D32f>& xs2);

	CvPoint3D32f scalePoint3D(CvPoint3D32f p3d, double v);

	CvMat* getLine(CvPoint2D32f p1, CvPoint2D32f p2);

	CvMat* getLine(CvPoint p1, CvPoint p2);

	//input four homogeneous coordinates of 2D point
	//0, 1 defines first line, 2, 3 defines second line
	CvMat* getIntersectionPoint(const vector<CvMat*> ps);

	CvPoint3D32f normalizePoint3D(const CvPoint3D32f& p);

	//p2 - p1
	CvPoint3D32f subPoint(const CvPoint3D32f& p1, const CvPoint3D32f& p2);

	//p2 + p1
	CvPoint3D32f addPoint(const CvPoint3D32f& p1, const CvPoint3D32f& p2);

	double getPointLength(const CvPoint3D32f& p);

	double getPointLength(const CvPoint3D64f& p);

	CvPoint2D32f getCentroid(const vector<CvPoint2D32f>& ps);

	//ps + t
	vector<CvPoint2D32f> movePoints(const vector<CvPoint2D32f>& ps, const CvPoint2D32f& t);

	//acos(p1*p2/||p1||x||p2||)
	double getPointAngle(const CvPoint3D32f& p1, const CvPoint3D32f& p2);

	/**
	 * Normalized the points. 
	 * 1) make the center to the origin. by translation
	 * 2) scale to make the mean distance to be sqrt(2.0). 
	 * 
	 * nX = T * X
	 * T = [w, 0, -w*mx; 0, w, -w*my; 0, 0, 1]
	 * mx = sum(x)/n; my = sum(y)/n
	 * w = sqrt(2.0) / (sqrt(sum((x-mx)^2 + (y-my)^2))/n)
	 */
	CvMat* normalize(const vector<CvPoint2D32f>& xs, vector<CvPoint2D32f>& nxs, CvMat* buf);

	//operator overloading
	CvPoint operator+ (const CvPoint& p1, const CvPoint& p2);
	CvPoint2D32f operator+ (const CvPoint2D32f& p1, const CvPoint2D32f& p2);
	CvPoint operator- (const CvPoint& p1, const CvPoint& p2);
	CvPoint2D32f operator- (const CvPoint2D32f& p1, const CvPoint2D32f& p2);
	CvPoint operator* (const CvPoint& p1, double v);
	CvPoint2D32f operator* (const CvPoint2D32f& p1, double v);
	CvPoint operator/ (const CvPoint& p1, double v);
	CvPoint2D32f operator/ (const CvPoint2D32f& p1, double v);

	vector<CvPoint2D32f> operator+ (const vector<CvPoint2D32f>& pts, const CvPoint2D32f& tp);



	CvSize operator* (const CvSize& size, double v);
	CvSize operator/ (const CvSize& size, double v);

	CvRect operator* (const CvRect& rect, double v);
	CvRect operator/ (const CvRect& rect, double v);


	//========= Operators for CvPoint3D32f ===================>
	CvPoint3D32f operator+ (const CvPoint3D32f& p1, const CvPoint3D32f& p2);
	CvPoint3D32f operator- (const CvPoint3D32f& p1, const CvPoint3D32f& p2);
	CvPoint3D32f operator* (const CvPoint3D32f& p1, double v);
	CvPoint3D32f operator/ (const CvPoint3D32f& p1, double v);

	vector<CvPoint3D32f> operator* (const vector<CvPoint3D32f>& ps, double v);

	/** Dot product
	 */
	double operator* (const CvPoint3D32f& p1, const CvPoint3D32f& p2);

	//========= Operators for CvPoint3D64f ===================>
	CvPoint3D64f operator+ (const CvPoint3D64f& p1, const CvPoint3D64f& p2);
	CvPoint3D64f operator- (const CvPoint3D64f& p1, const CvPoint3D64f& p2);
	CvPoint3D64f operator* (const CvPoint3D64f& p1, double v);
	CvPoint3D64f operator/ (const CvPoint3D64f& p1, double v);


	/**
	 * R = [cos(tehta), -sin(theta);
	 *      sin(theta),  cos(theta)]
	 * p' = R * p
	 *
	 */
	CvPoint2D32f rotatePoint(const CvPoint2D32f& p, double theta);

	///p' = R * p; R is a 3x3 matrix
	CvPoint3D64f rotatePoint(const CvPoint3D64f& p, const CvMat* R);

	//if the line do not inside the size, return false and let p1 = (-1, -1) and p2 = (-1, -1)
	bool computeLineInBoundary(const CvMat* line, const CvSize& size, CvPoint2D32f& p1, CvPoint2D32f& p2);
}
