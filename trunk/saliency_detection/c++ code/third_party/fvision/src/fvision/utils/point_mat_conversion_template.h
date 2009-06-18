#ifndef FVISION_POINT_MAT_CONVERSION_TEMPLATE_H_
#define FVISION_POINT_MAT_CONVERSION_TEMPLATE_H_

#pragma warning (disable : 4244)

#include <cv.h>

#include <cassert>
#include <vector>

namespace fvision {

// if m == NULL, create a new mat and return
// otherwise, write to m and return m
template<typename TPoint2D>
CvMat* hmatFromPoint2D(const TPoint2D& p, CvMat* m = NULL) {
	if (m == NULL) {
		m = cvCreateMat(3, 1, CV_64FC1);
	}
	cvmSet(m, 0, 0, p.x);
	cvmSet(m, 1, 0, p.y);
	cvmSet(m, 2, 0, 1.0);
	return m;
}

template<typename TPoint2D>
CvMat* matFromPoint2D(const TPoint2D& p, CvMat* m = NULL) {
	if (m == NULL) {
		m = cvCreateMat(2, 1, CV_64FC1);
	}
	cvmSet(m, 0, 0, p.x);
	cvmSet(m, 1, 0, p.y);
	return m;
}

template<typename TPoint2D>
void hmatToPoint2D(const CvMat* m, TPoint2D& p) {
	assert(m != NULL && m->rows == 3 && m->cols == 1);
	p.x = cvmGet(m, 0, 0) / cvmGet(m, 2, 0);
	p.y = cvmGet(m, 1, 0) / cvmGet(m, 2, 0);
}

template<typename TPoint2D>
void matToPoint2D(const CvMat* m, TPoint2D& p) {
	assert(m != NULL && m->rows == 2 && m->cols == 1);
	p.x = cvmGet(m, 0, 0);
	p.y = cvmGet(m, 1, 0);
}

template<typename TPoint2D>
void vectorHmatFromPoint2D(const std::vector<TPoint2D>& ps, std::vector<CvMat*>& mats) {
	mats.resize(ps.size());
	for (size_t i = 0; i < ps.size(); i++) {
		mats[i] = hmatFromPoint2D(ps[i]);
	}
}

template<typename TPoint2D>
void vectorHmatToPoint2D(const std::vector<CvMat*>& mats, std::vector<TPoint2D>& ps) {
	ps.resize(mats.size());
	for (size_t i = 0; i < ps.size(); i++) {
		ps[i] = hmatToPoint2D(mats[i]);
	}
}

//2 x N
template<typename TPoint2D>
CvMat* matFromPoint2DVector(const std::vector<TPoint2D>& xs, CvMat* m = NULL) {
	int N = (int)xs.size();
	if (m != NULL) {
		assert(m->rows == 2 && m->cols == N);
	} else {
		m = cvCreateMat(2, N, CV_64FC1);
	}
	for (int i = 0; i < N; i++) {
		cvmSet(m, 0, i, xs[i].x);
		cvmSet(m, 1, i, xs[i].y);
	}
	return m;
}

//3 x N
template<typename TPoint2D>
CvMat* hmatFromPoint2DVector(const std::vector<TPoint2D>& xs, CvMat* m = NULL) {
	int N = (int)xs.size();
	if (m != NULL) {
		assert(m->rows == 3 && m->cols == N);
	} else {
		m = cvCreateMat(3, N, CV_64FC1);
	}
	for (int i = 0; i < N; i++) {
		cvmSet(m, 0, i, xs[i].x);
		cvmSet(m, 1, i, xs[i].y);
		cvmSet(m, 2, i, 1.0);
	}
	return m;
}

//2 x N
template<typename TPoint2D>
void matToPoint2DVector(const CvMat* m, std::vector<TPoint2D>& xs) {
	assert(m != NULL && m->rows == 2);
	int N = m->cols;
	xs.resize(N);
	for (int i = 0; i < N; i++) {
		xs[i].x = cvmGet(m, 0, i);
		xs[i].y = cvmGet(m, 1, i);
	}
}

//3 x N
template<typename TPoint2D>
void hmatToPoint2DVector(const CvMat* m, std::vector<TPoint2D>& xs) {
	assert(m != NULL && m->rows == 2);
	int N = m->cols;
	xs.resize(N);
	for (int i = 0; i < N; i++) {
		xs[i].x = cvmGet(m, 0, i) / cvmGet(m, 2, i);
		xs[i].y = cvmGet(m, 1, i) / cvmGet(m, 2, i);
	}
}

//======================= Point3D ==================================>
template<typename TPoint3D>
CvMat* hmatFromPoint3D(const TPoint3D& p, CvMat* m = NULL) {
	if (m == NULL) {
		m = cvCreateMat(4, 1, CV_64FC1);
	}
	cvmSet(m, 0, 0, p.x);
	cvmSet(m, 1, 0, p.y);
	cvmSet(m, 2, 0, p.z);
	cvmSet(m, 3, 0, 1.0);
	return m;
}

template<typename TPoint3D>
CvMat* matFromPoint3D(const TPoint3D& p, CvMat* m = NULL) {
	if (m == NULL) {
		m = cvCreateMat(3, 1, CV_64FC1);
	}
	cvmSet(m, 0, 0, p.x);
	cvmSet(m, 1, 0, p.y);
	cvmSet(m, 2, 0, p.z);
	return m;
}

template<typename TPoint3D>
void hmatToPoint3D(const CvMat* m, TPoint3D& p) {
	assert(m != NULL && m->rows == 4 && m->cols == 1);
	p.x = cvmGet(m, 0, 0) / cvmGet(m, 3, 0);
	p.y = cvmGet(m, 1, 0) / cvmGet(m, 3, 0);
	p.z = cvmGet(m, 2, 0) / cvmGet(m, 3, 0);
}

template<typename TPoint3D>
void matToPoint3D(const CvMat* m, TPoint3D& p) {
	assert(m != NULL && m->rows == 3 && m->cols == 1);
	p.x = cvmGet(m, 0, 0);
	p.y = cvmGet(m, 1, 0);
	p.z = cvmGet(m, 2, 0);
}

template<typename TPoint3D>
void vectorHmatFromPoint3D(const std::vector<TPoint3D>& ps, std::vector<CvMat*>& mats) {
	mats.resize(ps.size());
	for (size_t i = 0; i < ps.size(); i++) {
		mats[i] = hmatFromPoint3D(ps[i]);
	}
}

template<typename TPoint3D>
void vectorHmatToPoint3D(const std::vector<CvMat*>& mats, std::vector<TPoint3D>& ps) {
	ps.resize(mats.size());
	for (size_t i = 0; i < ps.size(); i++) {
		ps[i] = hmatToPoint3D(mats[i]);
	}
}

//2 x N
template<typename TPoint3D>
CvMat* matFromPoint3DVector(const std::vector<TPoint3D>& xs, CvMat* m = NULL) {
	int N = (int)xs.size();
	if (m != NULL) {
		assert(m->rows == 3 && m->cols == N);
	} else {
		m = cvCreateMat(3, N, CV_64FC1);
	}
	for (int i = 0; i < N; i++) {
		cvmSet(m, 0, i, xs[i].x);
		cvmSet(m, 1, i, xs[i].y);
		cvmSet(m, 2, i, xs[i].z);
	}
	return m;
}

//3 x N
template<typename TPoint3D>
CvMat* hmatFromPoint3DVector(const std::vector<TPoint3D>& xs, CvMat* m = NULL) {
	int N = (int)xs.size();
	if (m != NULL) {
		assert(m->rows == 4 && m->cols == N);
	} else {
		m = cvCreateMat(4, N, CV_64FC1);
	}
	for (int i = 0; i < N; i++) {
		cvmSet(m, 0, i, xs[i].x);
		cvmSet(m, 1, i, xs[i].y);
		cvmSet(m, 2, i, xs[i].z);
		cvmSet(m, 3, i, 1.0);
	}
	return m;
}

//2 x N
template<typename TPoint3D>
void matToPoint3DVector(const CvMat* m, std::vector<TPoint3D>& xs) {
	assert(m != NULL && m->rows == 3);
	int N = m->cols;
	xs.resize(N);
	for (int i = 0; i < N; i++) {
		xs[i].x = cvmGet(m, 0, i);
		xs[i].y = cvmGet(m, 1, i);
		xs[i].z = cvmGet(m, 2, i);
	}
}

//3 x N
template<typename TPoint3D>
void hmatToPoint3DVector(const CvMat* m, std::vector<TPoint3D>& xs) {
	assert(m != NULL && m->rows == 3);
	int N = m->cols;
	xs.resize(N);
	for (int i = 0; i < N; i++) {
		xs[i].x = cvmGet(m, 0, i) / cvmGet(m, 3, i);
		xs[i].y = cvmGet(m, 1, i) / cvmGet(m, 3, i);
		xs[i].z = cvmGet(m, 2, i) / cvmGet(m, 3, i);
	}
}


}
#endif // FVISION_POINT_MAT_CONVERSION_TEMPLATE_H_