#pragma once

#include <cv.h>

#include <vector>

namespace fvision {

class PointTransformer {
public:
	PointTransformer(void);
	virtual ~PointTransformer(void);

public:
	virtual CvPoint2D32f transform(const CvPoint2D32f& p) const = 0;

	CvPoint2D32f operator() (const CvPoint2D32f& p) const {
		return transform(p);
	}

	std::vector<CvPoint2D32f> copyTransform(const std::vector<CvPoint2D32f>& ps) const ;

	void modifyTransform(std::vector<CvPoint2D32f>& ps) const ;

};


}