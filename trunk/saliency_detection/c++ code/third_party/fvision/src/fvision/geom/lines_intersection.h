#pragma once

#include <cv.h>

#include <fvision/utils.h>
#include <vector>

namespace fvision {

class LinesIntersection {
public:
	virtual void addNewRay(double a, double b, double c) = 0;

	void addNewRayMat(const CvMat* ray) {
		addNewRay(cvmGet(ray, 0, 0), cvmGet(ray, 1, 0), cvmGet(ray, 2, 0));
	}

	virtual CvPoint2D64f getSolution() const = 0;

	virtual double getAverageError() const = 0;
};


/**
*	each time add a 2D ray, update the solution
*	Geometry: transpose(r) * p = 0
*	r is the homogeneous ray coords and p is the homogeneous point coords
*
*/
class LinesIntersectionDlt : public LinesIntersection {
public:
	LinesIntersectionDlt(void);
	~LinesIntersectionDlt(void);

public:
	void addNewRay(double a, double b, double c);

	CvPoint2D64f getSolution() const { return solution; }

	/// avg(m*m); m = transpose(r) * p, where ||p|| = 1
	double getAverageError() const { return error; }

private:
	void compute();

	friend std::ostream& operator<< (std::ostream& os, const LinesIntersectionDlt& pdlt);
	friend std::istream& operator>> (std::istream& is, LinesIntersectionDlt& pdlt);

private:
	std::vector<CvPoint3D64f> rayps;
	CvPoint2D64f solution;
	double error;
};

std::ostream& operator<< (std::ostream& os, const LinesIntersectionDlt& pdlt);
std::istream& operator>> (std::istream& is, LinesIntersectionDlt& pdlt);

class LinesIntersectionOnlineDlt : public LinesIntersection {
public:
	LinesIntersectionOnlineDlt(void);
	~LinesIntersectionOnlineDlt(void) {}

public:
	void addNewRay(double a, double b, double c);

	CvPoint2D64f getSolution() const { return solution; }

	/// avg(m*m); m = transpose(r) * p, where ||p|| = 1
	double getAverageError() const { return error; }

	LinesIntersectionOnlineDlt* clone() const;

private:
	/// solution is the last column of svd
	void computeSolutionWithAtA();

	void initializeAtA();
	void addANewRayToAtA(double a, double b, double c);

	friend std::ostream& operator<< (std::ostream& os, const LinesIntersectionOnlineDlt& pdlt);
	friend std::istream& operator>> (std::istream& is, LinesIntersectionOnlineDlt& pdlt);

private:
	CvMatWrapper AtA; //a 3x3 matrix, update it each time and get solution from svd
	CvPoint2D64f solution;
	int raysSize;
	double error;
};

std::ostream& operator<< (std::ostream& os, const LinesIntersectionOnlineDlt& pdlt);
std::istream& operator>> (std::istream& is, LinesIntersectionOnlineDlt& pdlt);

//connect the intersection point to the mid point of the line segment, compute distance from end point to the line
double computePointPairsIntersectionGeometricError(const CvMat* solution, const std::vector<CvPoint2D32f>& p1s, const std::vector<CvPoint2D32f>& p2s);

/**
 * given a set of point pairs
 * compute the intersection point and its uncertainty
 * can be used to compute vanishing point, epipole
 */
class UncertainPointPairsIntersection {
public:
	UncertainPointPairsIntersection();
	~UncertainPointPairsIntersection();

public:
	void compute(const std::vector<CvPoint2D32f>& p1s, const std::vector<CvPoint2D32f>& p2s, double stdDev);

	double getOptimalAlgebraicError() { return eigenValues[2]; }
	//connect the intersection point to the mid point of a point pair
	//compute the distance of each point to the line
	double getOptimalAverageGeometricError() { return avgGeometricErrors[2]; }
	const CvMat* getOptimalIntersection() { return eigenVectors[2]; }

	double getSingularValue(int index);

	const CvMat* getEigenVector(int index);

	double getAverageGeometricError(int index);

private:
	//if |p1 - p2| < stdDev, discard the constraint
	void addAConstraint(CvMat* AtA, const CvPoint2D32f& p1, const CvPoint2D32f& p2, double stdDev);
	

private:
	CvMat* eigenVectors[3];
	double eigenValues[3];
	double avgGeometricErrors[3];
};

}
