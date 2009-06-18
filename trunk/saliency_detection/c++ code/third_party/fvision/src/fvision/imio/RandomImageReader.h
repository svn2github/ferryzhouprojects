#pragma once

#include "PathParams.h"

#include <cv.h>

namespace fvision {

class RandomImageReader {
public:
	RandomImageReader(const PathParams& pathParams);
	~RandomImageReader(void);

public:
	IplImage* getImage(int frameId);

private:
	PathParams pathParams;
};

}