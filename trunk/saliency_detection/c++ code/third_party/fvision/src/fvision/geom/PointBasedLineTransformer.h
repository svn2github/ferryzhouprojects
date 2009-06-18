#pragma once

#include "LineTransformer.h"
#include "PointTransformer.h"

namespace fvision {

class PointBasedLineTransformer : public LineTransformer {
public:
	//not responsible for deleting pointTransformer
	PointBasedLineTransformer(const PointTransformer* pointTransformer);
	virtual ~PointBasedLineTransformer(void);

private:
	PointBasedLineTransformer(const PointBasedLineTransformer& rhs) {}
	PointBasedLineTransformer& operator= (const PointBasedLineTransformer& rhs) {}

public:
	Line transform(const Line& line) const;

private:
	const PointTransformer* pointTransformer;
};

}
