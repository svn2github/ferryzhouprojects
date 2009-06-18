#include <fvision/utils/point_transform_template.h>

#include <cv.h>

#include <cassert>
#include <vector>

namespace fvision {

template<typename TPoint2D, typename TPoint3D>
void projectPoint3DWithMatrix33(const CvMat* M, const TPoint3D& p3d, TPoint2D& p2d) {
	assert(M->rows == 3 && M->cols == 3);
	CvMat* mp3d = cvCreateMat(3, 1, CV_64FC1);
	CvMat* hmp2d = cvCreateMat(3, 1, CV_64FC1);
	matFromPoint3D(p3d, mp3d);
	cvMatMul(M, mp3d, hmp2d);
	hmatToPoint2D(hmp2d, p2d);
	cvReleaseMat(&hmp2d);
	cvReleaseMat(&mp3d);
}

template<typename TPoint2D, typename TPoint3D>
void projectPoint3DWithMatrix34(const CvMat* P, const TPoint3D& p3d, TPoint2D& p2d) {
	assert(M->rows == 3 && M->cols == 4);
	CvMat* hmp3d = cvCreateMat(4, 1, CV_64FC1);
	CvMat* hmp2d = cvCreateMat(3, 1, CV_64FC1);
	hmatFromPoint3D(p3d, hmp3d);
	cvMatMul(P, hmp3d, hmp2d);
	hmatToPoint2D(hmp2d, p2d);
	cvReleaseMat(&hmp2d);
	cvReleaseMat(&hmp3d);
}

template<typename TPoint2D, typename TPoint3D>
void projectPoint3DVectorWithMatrix33(const CvMat* M, const std::vector<TPoint3D>& p3ds, std::vector<TPoint2D>& p2ds) {
	assert(M->rows == 3 && M->cols == 3);
	CvMat* mp3d = cvCreateMat(3, 1, CV_64FC1);
	CvMat* hmp2d = cvCreateMat(3, 1, CV_64FC1);
	p2ds.resize(p3ds.size());
	for (size_t i = 0; i < p3ds.size(); i++) {
		matFromPoint3D(p3ds[i], mp3d);
		cvMatMul(M, mp3d, hmp2d);
		hmatToPoint2D(hmp2d, p2ds[i]);
	}
	cvReleaseMat(&hmp2d);
	cvReleaseMat(&mp3d);
}

template<typename TPoint2D, typename TPoint3D>
void projectPoint3DVectorWithMatrix34(const CvMat* P, const std::vector<TPoint3D>& p3ds, std::vector<TPoint2D>& p2d) {
	assert(M->rows == 3 && M->cols == 4);
	CvMat* hmp3d = cvCreateMat(4, 1, CV_64FC1);
	CvMat* hmp2d = cvCreateMat(3, 1, CV_64FC1);
	p2ds.resize(p3ds.size());
	for (size_t i = 0; i < p3ds.size(); i++) {
		hmatFromPoint3D(p3ds[i], hmp3d);
		cvMatMul(P, hmp3d, hmp2d);
		hmatToPoint2D(hmp2d, p2ds[i]);
	}
	cvReleaseMat(&hmp2d);
	cvReleaseMat(&hmp3d);
}


}