#include "lines_intersection.h"

namespace fvision {

LinesIntersectionDlt::LinesIntersectionDlt(void)
{
}

LinesIntersectionDlt::~LinesIntersectionDlt(void)
{
}

void LinesIntersectionDlt::addNewRay(double a, double b, double c) {
	CvPoint3D64f rayp = cvPoint3D64f(a, b, c);
	rayps.push_back(rayp);
	if (rayps.size() == 1) {
		solution.x = 0;
		solution.y = - rayp.z / rayp.y;
		error = 0;
	} else {
		compute();
	}
}

void LinesIntersectionDlt::compute() {
	int rows = (int)rayps.size();
	CvMat* bigMat = cvCreateMat(rows, 3, CV_64FC1);
	for (int i = 0; i < rows; i++) {
		cvmSet(bigMat, i, 0, rayps[i].x);
		cvmSet(bigMat, i, 1, rayps[i].y);
		cvmSet(bigMat, i, 2, rayps[i].z);
	}
	CvMat* X = null(bigMat);
	solution.x = cvmGet(X, 0, 0) / cvmGet(X, 2, 0);
	solution.y = cvmGet(X, 1, 0) / cvmGet(X, 2, 0);
	CvMat* E = matMul(bigMat, X);
	error = cvNorm(E) / rayps.size();
	cvReleaseMat(&bigMat);
	cvReleaseMat(&X);
	cvReleaseMat(&E);
}

std::ostream& operator<<(std::ostream& os, const LinesIntersectionDlt& pdlt) {
	os<<pdlt.solution<<" "<<pdlt.error<<" "<<pdlt.rayps.size();
	for (size_t i = 0; i < pdlt.rayps.size(); i++) {
		os<<" "<<pdlt.rayps[i];
	}
	return os;
}

std::istream& operator>>(std::istream& is, LinesIntersectionDlt& pdlt) {
	is>>pdlt.solution>>pdlt.error;
	int size;
	is>>size;
	for (int i = 0; i < size; i++) {
		CvPoint3D64f ray;
		is>>ray;
		pdlt.rayps.push_back(ray);
	}
	return is;
}

std::istream& operator>>(std::istream& is, LinesIntersectionOnlineDlt& pdlt) {
	return is;
}


void LinesIntersectionOnlineDlt::addNewRay(double a, double b, double c) {

	addANewRayToAtA(a, b, c);
	raysSize++;

	if (raysSize == 0) { //the first ray
		solution.x = 0;
		solution.y = - a / b;
		error = 0;
	} else {
		computeSolutionWithAtA();
	}
}

void LinesIntersectionOnlineDlt::initializeAtA() {
	CvMat* Zero = cvCreateMat(3, 3, CV_64FC1);
	cvZero(Zero);
	AtA = Zero;
	cvReleaseMat(&Zero);
}

void LinesIntersectionOnlineDlt::addANewRayToAtA(double a, double b, double c) {
	CvMat* m = AtA.getMat();
	double t[] = {a, b, c};
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cvmSet(m, i, j, cvmGet(m, i, j) + t[i] * t[j]);
		}
	}
}

void LinesIntersectionOnlineDlt::computeSolutionWithAtA() {

	CvMat* X = null(AtA.getMat());
	solution.x = cvmGet(X, 0, 0) / cvmGet(X, 2, 0);
	solution.y = cvmGet(X, 1, 0) / cvmGet(X, 2, 0);
	CvMat* E = matMul(AtA.getMat(), X);
	error = cvNorm(E) / raysSize;
	cvReleaseMat(&X);
	cvReleaseMat(&E);
}

LinesIntersectionOnlineDlt::LinesIntersectionOnlineDlt(void) {
	raysSize = 0;
	initializeAtA();
}

LinesIntersectionOnlineDlt* LinesIntersectionOnlineDlt::clone() const {
	LinesIntersectionOnlineDlt* intersection = new LinesIntersectionOnlineDlt();
	intersection->AtA = this->AtA;
	intersection->solution = this->solution;
	intersection->raysSize = this->raysSize;
	intersection->error = this->error;
	return intersection;
}

std::ostream& operator<<(std::ostream& os, const LinesIntersectionOnlineDlt& pdlt) {
	return os;	
}

void UncertainPointPairsIntersection::compute(const std::vector<CvPoint2D32f>& p1s, const std::vector<CvPoint2D32f>& p2s, double stdDev) {
	assert(p1s.size() == p2s.size());
	CvMat* AtA = cvCreateMat(3, 3, CV_64FC1);
	cvZero(AtA);
	for (size_t i = 0; i < p1s.size(); i++) {
		addAConstraint(AtA, p1s[i], p2s[i], stdDev);
	}
	CvMat* W = cvCreateMat(3, 3, CV_64FC1);
	CvMat* U = cvCreateMat(3, 3, CV_64FC1);
	CvMat* V = cvCreateMat(3, 3, CV_64FC1);
	cvSVD(AtA, W, U, V);
	for (int i = 0; i < 3; i++) {
		copyCol(V, eigenVectors[i], i);
		eigenValues[i] = cvmGet(W, i, i);
		avgGeometricErrors[i] = computePointPairsIntersectionGeometricError(eigenVectors[i], p1s, p2s);
	}
	cvReleaseMat(&W);
	cvReleaseMat(&U);
	cvReleaseMat(&V);
}

UncertainPointPairsIntersection::UncertainPointPairsIntersection() {
	for (int i = 0; i < 3; i++) {
		eigenVectors[i] = cvCreateMat(3, 1, CV_64FC1);
	}	
}

UncertainPointPairsIntersection::~UncertainPointPairsIntersection() {
	for (int i = 0; i < 3; i++) {
		cvReleaseMat(&(eigenVectors[i]));
	}	
}

void UncertainPointPairsIntersection::addAConstraint(CvMat* AtA, const CvPoint2D32f& p1, const CvPoint2D32f& p2, double stdDev) {
	double length = pointDistance(p1, p2);
	if (length < stdDev) return;
	double ray[3] = {p1.y - p2.y, p2.x - p1.x, p1.x * p2.y - p1.y * p2.x};
	double weight = length * length;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cvmSet(AtA, i, j, cvmGet(AtA, i, j) + ray[i] * ray[j] * weight);
		}
	}
}

double computePointPairsIntersectionGeometricError(const CvMat* solution, const std::vector<CvPoint2D32f>& p1s, const std::vector<CvPoint2D32f>& p2s) {
	double avgGeometricError = 0;
	if (p1s.size() == 0) {
		return avgGeometricError;
	}
	CvMat* midPointBuf = cvCreateMat(3, 1, CV_64FC1);
	cvmSet(midPointBuf, 2, 0, 1.0);
	CvMat* lineBuf = cvCreateMat(3, 1, CV_64FC1);
	double totalError = 0;
	for (size_t i = 0; i < p1s.size(); i++) {
		cvmSet(midPointBuf, 0, 0, (p1s[i].x + p2s[i].x) / 2);
		cvmSet(midPointBuf, 1, 0, (p1s[i].y + p2s[i].y) / 2);
		cvCrossProduct(solution, midPointBuf, lineBuf);
		double dist = point2LineDistance(p1s[i], lineBuf);
		totalError += dist;
	}
	avgGeometricError = totalError * 2 / p1s.size();
	return avgGeometricError;
}

double UncertainPointPairsIntersection::getSingularValue(int index) { 
	assert (index >= 0 && index < 3);
	return eigenValues[index]; 
}

const CvMat* UncertainPointPairsIntersection::getEigenVector(int index) {
	assert (index >= 0 && index < 3);
	return eigenVectors[index]; 
}

double UncertainPointPairsIntersection::getAverageGeometricError(int index) {
	assert (index >= 0 && index < 3);
	return avgGeometricErrors[index]; 
}
}