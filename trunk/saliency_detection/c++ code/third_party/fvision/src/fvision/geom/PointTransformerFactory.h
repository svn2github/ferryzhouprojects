#pragma once

#include "PointTransformer.h"

namespace fvision {

class PointTransformerFactory {
public:
	PointTransformerFactory(void);
	~PointTransformerFactory(void);

public:
	///x' = inv(K) * x
	///x' = ifx * (x - px), y' = ify * (y - py)
	PointTransformer* createKNormalizer(const CvMat* K) const ;

	///x' = K * x
	///x' = fx * x + px, y' = fy * y + py
	PointTransformer* createKDeNormalizer(const CvMat* K) const ;

	///x' = x + t
	PointTransformer* createTranslator(const CvPoint2D32f& t) const ;

	///x' = H * x
	PointTransformer* createHomographyTransformer(const CvMat* H) const ;
};

}