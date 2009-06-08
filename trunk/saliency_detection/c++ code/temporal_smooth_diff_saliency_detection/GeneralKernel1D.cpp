#include "GeneralKernel1D.h"

#include <cassert>

GeneralKernel1D::GeneralKernel1D(double* mask, size_t size) {
	this->maskLength = size;
	this->mask = mask;
}

GeneralKernel1D::~GeneralKernel1D(void) {
}


GeneralKernel1D* GeneralKernel1D::createMeanSmoothKernel(size_t length) {
	assert (length % 2 == 1);
	double* mask = new double[length];
	for (size_t i = 0; i < length; i++) {
		mask[i] = 1.0 / length;
	}
	return new GeneralKernel1D(mask, length);
}

uchar GeneralKernel1D::filter(uchar* values) const {
	double sum = 0;	
	for (size_t i = 0; i < maskLength; i++) {
		sum += mask[i] * values[i];
	}
	return (uchar)sum;
}