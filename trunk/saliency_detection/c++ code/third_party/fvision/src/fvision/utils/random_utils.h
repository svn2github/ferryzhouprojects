#pragma once

#include <cv.h>

#include <vector>

namespace fvision {

float ranf();

float ranf(float min, float max);

double random();

double random(double min, double max);

int rani(int max);

/* normal random variate generator */
/* mean m, standard deviation s */
float gaussianRand(float m, float s);

//pick num points in range [0, max)
std::vector<int> ranis(int max, int num);

CvPoint3D64f randomPoint3D64f();

CvPoint2D32f gaussianRandomPoint2D32f(const CvPoint2D32f& mean, const CvPoint2D32f& cov);

void randomMat(CvMat* M);

CvMat* createRandomMat(int rows, int cols);
}