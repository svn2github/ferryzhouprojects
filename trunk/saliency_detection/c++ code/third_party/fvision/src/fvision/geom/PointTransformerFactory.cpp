#include "PointTransformerFactory.h"

#include "KNormalizer.h"
#include "KDeNormalizer.h"
#include "PointTranslator.h"
#include "HomographyPointTransformer.h"

namespace fvision {

PointTransformerFactory::PointTransformerFactory(void)
{
}

PointTransformerFactory::~PointTransformerFactory(void)
{
}

PointTransformer* PointTransformerFactory::createKNormalizer(const CvMat* K) const {
	return new KNormalizer(K);
}

PointTransformer* PointTransformerFactory::createKDeNormalizer(const CvMat* K) const {
	return new KDeNormalizer(K);
}

PointTransformer* PointTransformerFactory::createTranslator(const CvPoint2D32f& t) const {
	return new PointTranslator(t);
}

PointTransformer* PointTransformerFactory::createHomographyTransformer(const CvMat* H) const {
	return new HomographyPointTransformer(H);
}

}