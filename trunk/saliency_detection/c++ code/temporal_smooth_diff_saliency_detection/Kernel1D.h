#pragma once

#include <cv.h>

class Kernel1D {
public:
	Kernel1D(void);
	~Kernel1D(void);

public:
	virtual size_t size() const = 0;
	//assert the values size is the same as size()
	//return sum(mask[i] * values[i] | i = 0 ... size())
	virtual uchar filter(uchar* values) const = 0;
};
