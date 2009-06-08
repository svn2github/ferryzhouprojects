#include "ImageBlockFeature.h"

#include <cmath>
#include <cstring>

ImageBlockFeature::ImageBlockFeature(const double* values, size_t length, size_t id) {
	this->length = length;
	this->values = new double[length];
	memcpy(this->values, values, sizeof(double) * length);
	this->id = id;
}

ImageBlockFeature::~ImageBlockFeature(void)
{
	delete values;
}

double ImageBlockFeature::computeDistance(const Feature* rhs) {
	const ImageBlockFeature* blockFeature = (const ImageBlockFeature*)rhs;
	double sum = 0;
	for (size_t i = 0; i < length; i++) {
		sum += fabs(values[i] - blockFeature->values[i]);
	}
	return sum / length;
}

Feature* ImageBlockFeature::clone() const {
	return new ImageBlockFeature(values, length, id);
}

void ImageBlockFeature::setValues(const double* values) {
	memcpy(this->values, values, sizeof(double) * length);
}