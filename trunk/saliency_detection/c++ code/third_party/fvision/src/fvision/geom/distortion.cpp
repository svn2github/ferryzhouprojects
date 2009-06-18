#include "distortion.h"

#include <fvision/utils.h>

#include "transform_points_utils.h"

namespace fvision {

	CvPoint2D32f compensateDistortion(const CvPoint2D32f& x_d, const CvMat* k) {

		assert(k != NULL);
		
		double k1 = cvmGet(k, 0, 0);
		double k2 = cvmGet(k, 1, 0);
		double p1 = cvmGet(k, 2, 0);
		double p2 = cvmGet(k, 3, 0);

		CvPoint2D32f xn = x_d; //initial guess

		for (int i = 0; i < 20; i++) {
			double r2 = xn.x * xn.x + xn.y * xn.y;
			double radial = 1 + k1 * r2 + k2 * r2 * r2;
			double dxx = 2 * p1 * xn.x * xn.y + p2 * (r2 + 2 * xn.x * xn.x);
			double dxy = 2 * p2 * xn.x * xn.y + p1 * (r2 + 2 * xn.y * xn.y);

			xn.x = (x_d.x - dxx) / radial;
			xn.y = (x_d.y - dxy) / radial;
		}

		return xn;
	}

	CvPoint2D32f distort(const CvPoint2D32f& xn, const CvMat* k) {
		
		assert(k != NULL);
		
		double k1 = cvmGet(k, 0, 0);
		double k2 = cvmGet(k, 1, 0);
		double p1 = cvmGet(k, 2, 0);
		double p2 = cvmGet(k, 3, 0);

		double r2 = xn.x * xn.x + xn.y * xn.y;
		double radial = 1 + k1 * r2 + k2 * r2 * r2;
		double dxx = 2 * p1 * xn.x * xn.y + p2 * (r2 + 2 * xn.x * xn.x);
		double dxy = 2 * p2 * xn.x * xn.y + p1 * (r2 + 2 * xn.y * xn.y);

		double xdx = xn.x * radial + dxx;
		double xdy = xn.y * radial + dxy;

		return cvPoint2D32f(xdx, xdy);
	}

	vector<CvPoint2D32f> compensateDistortion(const vector<CvPoint2D32f>& xds, const CvMat* k) {
		vector<CvPoint2D32f> oxs;
		for (size_t i = 0; i < xds.size(); i++) {
			oxs.push_back(compensateDistortion(xds[i], k));
		}
		return oxs;
	}

	vector<CvPoint2D32f> distort(const vector<CvPoint2D32f>& xns, const CvMat* k) {

		assert(k != NULL);
		
		vector<CvPoint2D32f> oxs;
		for (int i = 0; i < (int)xns.size(); i++) {
			oxs.push_back(distort(xns[i], k));
		}
		return oxs;
	}

	CvPoint2D32f compensateDistortion(const CvPoint2D32f& dx, const CvMat* k, const CvMat* K) {

		assert(k != NULL);
		assert(K != NULL);
		
		CvMat* invK = inv(K);
		CvPoint2D32f ndx = transformPoint(invK, dx);
		CvPoint2D32f undx = compensateDistortion(ndx, k);
		CvPoint2D32f ux = transformPoint(K, undx);
		cvReleaseMat(&invK);
		return ux;
	}

	CvPoint2D32f distort(const CvPoint2D32f& ux, const CvMat* k, const CvMat* K) {

		assert(k != NULL);
		assert(K != NULL);
		
		CvMat* invK = inv(K);
		CvPoint2D32f nux = transformPoint(invK, ux);
		CvPoint2D32f dnux = distort(nux, k);
		CvPoint2D32f dx = transformPoint(K, dnux);
		cvReleaseMat(&invK);
		return dx;
	}

	vector<CvPoint2D32f> compensateDistortion(const vector<CvPoint2D32f>& dxs, const CvMat* k, const CvMat* K) {

		assert(k != NULL);
		assert(K != NULL);
		
		CvMat* invK = inv(K);
		vector<CvPoint2D32f> ndxs = transformPoints(invK, dxs);
		vector<CvPoint2D32f> undxs = compensateDistortion(ndxs, k);
		vector<CvPoint2D32f> uxs = transformPoints(K, undxs);
		cvReleaseMat(&invK);
		return uxs;
	}

	vector<CvPoint2D32f> distort(const vector<CvPoint2D32f>& uxs, const CvMat* k, const CvMat* K) {

		assert(k != NULL);
		assert(K != NULL);
		
		CvMat* invK = inv(K);
		vector<CvPoint2D32f> nuxs = transformPoints(invK, uxs);
		vector<CvPoint2D32f> dnuxs = distort(nuxs, k);
		vector<CvPoint2D32f> dxs = transformPoints(K, dnuxs);
		cvReleaseMat(&invK);
		return dxs;
	}

	CvPoint2D32f compensateDistortion(const CvPoint2D32f& ix, const CvPoint2D32f& c, const CvMat* k) {

		assert(k != NULL);
		
		return compensateDistortion(ix - c, k) + c;
	}

	CvPoint2D32f compensateDistortionWithNoOffset(const CvPoint2D32f& ix, const CvPoint2D32f& c, const CvMat* k) {

		assert(k != NULL);

		return compensateDistortion(ix - c, k);
	}

	ImagePointUndistorter::ImagePointUndistorter(const CvPoint2D32f cameraCenter, CvMat* k) {
		init(cameraCenter, k);
	}

	ImagePointUndistorter::ImagePointUndistorter(
		const CvPoint2D32f cameraCenter,
		double k1,
		double k2,
		double k3,
		double k4) 
	{
		this->cameraCenter = cameraCenter;
		this->k = cvCreateMat(4, 1, CV_64FC1);
		setk(k1, k2, k3, k4);
	}

	ImagePointUndistorter::~ImagePointUndistorter(void) {
		cvReleaseMat(&k);
	}

	ImagePointUndistorter::ImagePointUndistorter(const ImagePointUndistorter& rhs) {
		init(rhs.cameraCenter, rhs.k);
	}

	ImagePointUndistorter& ImagePointUndistorter::operator= (const ImagePointUndistorter& rhs) {
		if (this == &rhs) return *this;
		if (k != NULL) cvReleaseMat(&k);
		init(rhs.cameraCenter, rhs.k);
		this->offset = rhs.offset;
		return *this;
	}

	void ImagePointUndistorter::init(const CvPoint2D32f& cameraCenter, CvMat* k) {
		assert(k != NULL);
		this->cameraCenter = cameraCenter;
		this->k            = cvCloneMat(k);
	}

	CvPoint2D32f ImagePointUndistorter::operator() (const CvPoint2D32f& p) const {
		if (offset) {
			return compensateDistortion(p - cameraCenter, k) + cameraCenter;
		} else {
			return compensateDistortion(p - cameraCenter, k);
		}
	}

	void ImagePointUndistorter::setImageCenter(const CvPoint2D32f& cameraCenter) {
		this->cameraCenter = cameraCenter;
	}

	void ImagePointUndistorter::setk(double k1, double k2, double k3, double k4) {
		cvmSet(k, 0, 0, k1);
		cvmSet(k, 1, 0, k2);
		cvmSet(k, 2, 0, k3);
		cvmSet(k, 3, 0, k4);
	}
}