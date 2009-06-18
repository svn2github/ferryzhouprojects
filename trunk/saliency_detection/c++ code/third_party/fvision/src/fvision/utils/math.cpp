#include "math.h"

#include <cassert>
#include <cv.h>

#include <algorithm>

using namespace std;

namespace fvision {

	double degreeToRadian(double degrees) {
		return degrees * CV_PI / 180;
	}

	double radianToDegrees(double radian) {
		return radian * 180 / CV_PI;
	}

	double getMean(const vector<double>& vs) {
		int times = (int)vs.size();
		if (times == 0) {
			return 0;
		}

		double sum = 0;
		for (int i = 0; i < times; i++) {
			sum += vs[i];
		}
		return sum / times;
	}

	float getMean(const vector<float>& vs) {
		int times = (int)vs.size();
		if (times == 0) {
			return 0;
		}

		double sum = 0;
		for (int i = 0; i < times; i++) {
			sum += vs[i];
		}
		return (float)(sum / times);
	}

	double getAbsMean(const std::vector<double>& vs) {
		int times = (int)vs.size();
		if (times == 0) {
			return 0;
		}

		double sum = 0;
		for (int i = 0; i < times; i++) {
			sum += abs(vs[i]);
		}
		return sum / times;
	}

	void doMeanCovStatistics(const vector<double>& vs, double& mean, double& scov) {
		int times = (int)vs.size();
		if (times == 0) {
			mean = 0;
			scov = 0;
			return;
		}

		double sum = 0;
		double covsum = 0;
		for (int i = 0; i < times; i++) {
			sum += vs[i];
		}
		mean = sum / times;

		double tm;
		for (int i = 0; i < times; i++) {
			tm = vs[i] - mean;
			covsum += tm * tm;
		}
		double cov = covsum / times;
		scov = sqrt(cov);
	}

	double normalizeAngleToHalfPi(double theta) {
		if (theta < -CV_PI / 2) return normalizeAngleToHalfPi(theta + CV_PI); //a trick!!!!!!, should be fixed
		else if (theta > CV_PI / 2) return normalizeAngleToHalfPi(theta - CV_PI);
		else return theta;
	}

	double normalizeAngleToPi(double theta) {
		if (theta < -CV_PI) return theta + 2 * CV_PI;
		else if (theta > CV_PI) return theta - 2 * CV_PI;
		else return theta;
	}

}