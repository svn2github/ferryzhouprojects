#pragma once

#include "Line.h"

#include <cv.h>

#include <vector>

namespace fvision {

std::vector<CvMat*> getLineMats(std::vector<Line>& lines);

}