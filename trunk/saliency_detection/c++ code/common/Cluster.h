#pragma once

#include "Feature.h"

class Cluster {
public:
	virtual ~Cluster(void);
	virtual bool isIn(const Feature* feature) = 0;
	virtual void add(const Feature* feature) = 0;
	virtual Cluster* create(const Feature* feature) = 0;
	virtual size_t size() const = 0;
};
