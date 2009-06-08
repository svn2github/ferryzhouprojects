#pragma once

#include "Feature.h"

class ImageBlockFeature : public Feature {
public:
	ImageBlockFeature(const double* values, size_t length, size_t id);
	~ImageBlockFeature(void);
public:
	double computeDistance(const Feature* rhs);
	Feature* clone() const;

public:
	void setValues(const double* values);

public:
	double* values;
	size_t length;
	size_t id;
};
