#ifndef FVISION_POINT_TRANSFORM_TEMPLATE_H_
#define FVISION_POINT_TRANSFORM_TEMPLATE_H_

#include "point_mat_conversion_template.h"
#include "template_utils.h"

#include <cv.h>

#include <cassert>
#include <vector>

namespace fvision {

template<typename TPoint2D>
class Point2DTransformer {
public:
	virtual ~Point2DTransformer(void) {}
	virtual TPoint2D transform(const TPoint2D& p) const = 0;

	TPoint2D operator() (const TPoint2D& p) const {
		return transform(p);
	}
};

template<typename TPoint2D>
class HomographyPoint2DTransformer : public Point2DTransformer<TPoint2D> {
public:
	HomographyPoint2DTransformer(const CvMat* H) {
		assert(H != NULL);
		assert(H->rows == 3 && H->cols == 3);
		this->H = cvCloneMat(H);
		this->inputPointBuffer = cvCreateMat(3, 1, CV_64FC1);
		this->outputPointBuffer = cvCreateMat(3, 1, CV_64FC1);
	}
	~HomographyPoint2DTransformer(void) {
		cvReleaseMat(&H);
		cvReleaseMat(&inputPointBuffer);
		cvReleaseMat(&outputPointBuffer);
	}

public:
	TPoint2D transform(const TPoint2D& p) const {
		hmatFromPoint2D(p, inputPointBuffer);
		cvMatMul(H, inputPointBuffer, outputPointBuffer);
		TPoint2D np;
		hmatToPoint2D(outputPointBuffer, np);
		return np;
	}

private:
	CvMat* H;

	CvMat* inputPointBuffer;
	CvMat* outputPointBuffer;
};


template<typename TPoint2D>
TPoint2D transformPoint2D(const CvMat* H, const TPoint2D& p) {
	HomographyPoint2DTransformer<TPoint2D> transformer(H);
	return transformer.transform(p);
}

template<typename TPoint2D>
std::vector<TPoint2D> copyTransformPoint2DVector(const CvMat* H, const std::vector<TPoint2D>& ps) {
	HomographyPoint2DTransformer<TPoint2D> transformer(H);
	return copyTransform(ps, &transformer);
}

template<typename TPoint2D>
void modifyTransformPoint2DVector(const CvMat* H, std::vector<TPoint2D>& ps) {
	HomographyPoint2DTransformer<TPoint2D> transformer(H);
	modifyTransform(ps, &transformer);
}

//======================= Point3D ==================================>

template<typename TPoint3D>
class Point3DTransformer {
public:
	virtual ~Point3DTransformer(void) {}
	virtual TPoint3D transform(const TPoint3D& p) const = 0;

	TPoint3D operator() (const TPoint3D& p) const {
		return transform(p);
	}
};

template<typename TPoint3D>
class HomographyPoint3DTransformer : public Point3DTransformer<TPoint3D> {
public:
	HomographyPoint3DTransformer(const CvMat* H) {
		assert(H != NULL);
		assert(H->rows == 4 && H->cols == 4);
		this->H = cvCloneMat(H);
		this->inputPointBuffer = cvCreateMat(4, 1, CV_64FC1);
		this->outputPointBuffer = cvCreateMat(4, 1, CV_64FC1);
	}
	~HomographyPoint3DTransformer(void) {
		cvReleaseMat(&H);
		cvReleaseMat(&inputPointBuffer);
		cvReleaseMat(&outputPointBuffer);
	}

public:
	TPoint3D transform(const TPoint3D& p) const {
		hmatFromPoint3D(p, inputPointBuffer);
		cvMatMul(H, inputPointBuffer, outputPointBuffer);
		TPoint3D np;
		hmatToPoint3D(outputPointBuffer, np);
		return np;
	}

private:
	CvMat* H;

	CvMat* inputPointBuffer;
	CvMat* outputPointBuffer;
};


template<typename TPoint3D>
TPoint3D transformPoint3D(const CvMat* H, const TPoint3D& p) {
	HomographyPoint3DTransformer<TPoint3D> transformer(H);
	return transformer.transform(p);
}

template<typename TPoint3D>
std::vector<TPoint3D> copyTransformPoint3DVector(const CvMat* H, const std::vector<TPoint3D>& ps) {
	HomographyPoint3DTransformer<TPoint3D> transformer(H);
	return copyTransform(ps, &transformer);
}

template<typename TPoint3D>
void modifyTransformPoint3DVector(const CvMat* H, std::vector<TPoint3D>& ps) {
	HomographyPoint3DTransformer<TPoint3D> transformer(H);
	modifyTransform(ps, &transformer);
}


}
#endif // FVISION_POINT_TRANSFORM_TEMPLATE_H_