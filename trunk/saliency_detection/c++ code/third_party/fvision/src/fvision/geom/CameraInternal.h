#pragma once

#include "PointTransformer.h"
#include "distortion.h"

namespace fvision {

/** camera interal parameters, including internal matrix and distortion parameters
 * 
 */
class CameraInternal {
public:
	/// K = I, k = NULL
	CameraInternal();
	CameraInternal(const CvMat* K, const CvMat* k = NULL);
	~CameraInternal(void);

public:
	CameraInternal(const CameraInternal& rhs);
	CameraInternal& operator= (const CameraInternal& rhs);

private:
	void initWith(const CvMat* K, const CvMat* k, bool toNormalize);

public:
	///normalize p
	CvPoint2D32f operator()(const CvPoint2D32f& p) const ;

	/**
	 * convert image point to normalized point
	 */
	CvPoint2D32f normalize(const CvPoint2D32f& p) const ;

	/**
	 * convert normalized point to image point
	 */
	CvPoint2D32f deNormalize(const CvPoint2D32f& np) const ;

	void setToNormalize(bool v) {
		this->toNormalize = v;
	}

	double getHorizontalFocalLength() const {
		return cvmGet(K, 0, 0);
	}

	double getVerticalFocalLength() const {
		return cvmGet(K, 1, 1);
	}

public:
	ImagePointUndistorter getImagePointUndistorter() const;

	const PointTransformer* getKDeNormalizer() const;

public:
	const CvMat* getInternalMatrix() const { return K; }
	const CvMat* getDistortionVector() const { return k; }

private:
	CvMat* K;
	CvMat* k;

	PointTransformer* pknorm;
	NormalizedPointUndistorter* pundist;
	PointTransformer* pkdenorm;

	//if true, the operator() do normalize; if false, do denormalize
	//default is true
	bool toNormalize;
};

}