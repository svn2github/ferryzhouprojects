#pragma once

#include "Line.h"

#include <cv.h>

#include <vector>

namespace fvision {

/// l' = H-t * l
std::vector<Line> transformLinesWithHomography(const CvMat* H, const std::vector<Line>& lines);

}