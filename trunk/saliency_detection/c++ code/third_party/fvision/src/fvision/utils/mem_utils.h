#pragma once

#include <cv.h>

#include <vector>

namespace fvision {

void releaseImageVector(std::vector<IplImage*>& images);

}