#pragma once

#include "Line.h"

#include <vector>

namespace fvision {

class LineTransformer {
public:
	LineTransformer(void);
	virtual ~LineTransformer(void);

public:
	virtual Line transform(const Line& line) const = 0;

	Line operator() (const Line& line) const {
		return transform(line);
	}

	std::vector<Line> copyTransform(const std::vector<Line>& lines) const ;

	void modifyTransform(std::vector<Line>& lines) const ;

};

}
