#pragma once

#include <cmath>
#include <stdlib.h>

#include <vector>
#include <cassert>

using namespace std;

namespace ferry { namespace util {

	//return v if minv <= v <= maxv
	//return minv if v < minv
	//return maxv if v > maxv
	double clip(double v, double minv, double maxv);

	float ranf();

	int rani(int max);

	/* normal random variate generator */
	/* mean m, standard deviation s */
	float gaussianRand(float m, float s);

	//pick num points in range [0, max)
	vector<int> ranis(int max, int num);


}}