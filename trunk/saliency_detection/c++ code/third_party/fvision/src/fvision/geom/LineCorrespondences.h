#pragma once

#include <fvision/geom.h>
#include <vector>
#include <iostream>

namespace fvision {

class LineCorrespondences {
public:
	LineCorrespondences(void);
	~LineCorrespondences(void);

public:
	std::vector<Line> lines1;
	std::vector<Line> lines2;
	std::vector<int> ids;
	int frameId1;
	int frameId2;
};

std::ostream& operator<< (std::ostream& os, const LineCorrespondences& lc);
std::istream& operator>> (std::istream& is, LineCorrespondences& lc);

}