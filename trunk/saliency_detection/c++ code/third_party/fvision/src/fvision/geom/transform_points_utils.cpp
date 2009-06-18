#include "transform_points_utils.h"

#include "PointTransformerFactory.h"

#include <fvision/utils/point_mat_conversion_template.h>
#include <fvision/utils/cvmat_utils.h>

using namespace std;

namespace fvision {

CvPoint2D32f transformPoint(const CvMat* H, const CvPoint2D32f& p) {
	CvMat* pm = hmatFromPoint2D(p);
	CvMat* tpm = matMul(H, pm);
	CvPoint2D32f tp;
	hmatToPoint2D(tpm, tp);
	cvReleaseMat(&pm);
	cvReleaseMat(&tpm);
	return tp;
}

vector<CvPoint2D32f> transformPoints(const CvMat* H, const vector<CvPoint2D32f>& xs) {
	PointTransformerFactory factory;
	PointTransformer* pt = factory.createHomographyTransformer(H);
	vector<CvPoint2D32f> nxs = pt->copyTransform(xs);
	delete pt;
	return nxs;
}

std::vector<CvPoint2D32f> normalizePointsWithCameraInternalMatrix(const CvMat* K, const std::vector<CvPoint2D32f>& xs) {
	PointTransformerFactory factory;
	PointTransformer* pt = factory.createKNormalizer(K);
	vector<CvPoint2D32f> nxs = pt->copyTransform(xs);
	delete pt;
	return nxs;
}

fvision::PointPairs transformPointPairs(const CvMat* H, const PointPairs& pps) {
	PointTransformerFactory factory;
	PointTransformer* transformer = factory.createHomographyTransformer(H);

	PointPairs npps;
	PointPairs::const_iterator iter = pps.begin();
	for (; iter != pps.end(); iter++) {
		CvPoint2D32f nx1 = transformer->transform(iter->first);
		CvPoint2D32f nx2 = transformer->transform(iter->second);
		npps.push_back(std::pair<CvPoint2D32f, CvPoint2D32f>(nx1, nx2));
	}
	return npps;

}

}