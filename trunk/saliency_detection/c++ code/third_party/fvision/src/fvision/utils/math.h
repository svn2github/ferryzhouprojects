#pragma once

#include <vector>
#include <cmath>

/*!
math functions
mainly probability functions, random sample

*/
namespace fvision {

	double degreeToRadian(double degrees);
	double radianToDegrees(double radian);

	void doMeanCovStatistics(const std::vector<double>& vs, double& mean, double& scov);
	double getMean(const std::vector<double>& vs);
	double getAbsMean(const std::vector<double>& vs);

	float getMean(const std::vector<float>& vs);

	// normalize theta so that it is in [-pi/2, pi/2]
	double normalizeAngleToHalfPi(double theta);

	// normalize theta so that it is in [-pi, pi]
	double normalizeAngleToPi(double theta);
}