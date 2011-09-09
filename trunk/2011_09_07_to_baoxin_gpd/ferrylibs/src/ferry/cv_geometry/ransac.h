#pragma once

#include <cmath>

namespace ferry {
	namespace math {

class RANSAC
{
public:
	RANSAC(int modelSampleSize) {
		this->s = modelSampleSize;
		this->p = 0.99;
	}
	~RANSAC(void) {}

public:
	int calcN(int inliersNumber, int samplesNumber) {
		double e = 1 - (double)inliersNumber / samplesNumber;
		//cout<<"e: "<<e<<endl;
		if (e > 0.9) e = 0.9;
		//cout<<"pow: "<<pow((1 - e), s)<<endl;
		//cout<<log(1 - pow((1 - e), s))<<endl;
		return (int)(log(1 - p) / log(1 - pow((1 - e), s)));
	}

private:
	//samples size for fitting a model
	int s;

	double p;

};

	}
}