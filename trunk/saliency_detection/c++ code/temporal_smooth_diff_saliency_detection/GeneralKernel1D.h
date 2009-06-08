#pragma once

#include "Kernel1D.h"

class GeneralKernel1D : public Kernel1D {
public:
	GeneralKernel1D(double* mask, size_t size);
	~GeneralKernel1D(void);

public:
	static GeneralKernel1D* createMeanSmoothKernel(size_t length);

public:
	size_t size() const { return maskLength; }
	//s = sum(mask[i] * values[i]);
	uchar filter(uchar* values) const ;

private:
	size_t maskLength;
	double* mask;
};
