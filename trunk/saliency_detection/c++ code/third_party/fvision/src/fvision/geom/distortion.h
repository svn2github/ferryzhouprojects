#pragma once

#include <cv.h>

#include <vector>

using std::vector;

/** the module of lens distortion
 *
 */
namespace fvision {

	//compensate lens distortion
	//input: 
	//x_d: normalized distorted image point
	//k:   4 x 1 matrix, distortion coefficients (radial and tangential)
	//output:
	//xn: normalized undistorted image point
	//
	//x_d = (1 + k1 * r^2 + k2 * r^4) * xn + dx
	//dx = [2 * p1 * x * y + p2 * (r^2 + 2 * x^2); 
	//      p1 * (r^2 + 2 * y^2) + 2 * p2 * x * y]
	//
	//Iteratively determine xn based x_d use the above equation
	//
	CvPoint2D32f compensateDistortion(const CvPoint2D32f& x_d, const CvMat* k);

	CvPoint2D32f distort(const CvPoint2D32f& xn, const CvMat* k);

	vector<CvPoint2D32f> compensateDistortion(const vector<CvPoint2D32f>& xds, const CvMat* k);
	vector<CvPoint2D32f> distort(const vector<CvPoint2D32f>& xns, const CvMat* k);


	//dx: distorted image point
	//K: camera internal matrix
	CvPoint2D32f compensateDistortion(const CvPoint2D32f& dx, const CvMat* k, const CvMat* K);
	//ux: undistorted image point
	CvPoint2D32f distort(const CvPoint2D32f& ux, const CvMat* k, const CvMat* K);

	vector<CvPoint2D32f> compensateDistortion(const vector<CvPoint2D32f>& dxs, const CvMat* k, const CvMat* K);
	vector<CvPoint2D32f> distort(const vector<CvPoint2D32f>& uxs, const CvMat* k, const CvMat* K);

	/**
	 * @param ix distorted image point
	 * @param c  image center
	 * @param k  distortion parameters 
	 * @return   undistorted image point
	 *
	 * Algorithm:
	 *   x' = undistort(ix - c, k) + c
	 */
	CvPoint2D32f compensateDistortion(const CvPoint2D32f& ix, const CvPoint2D32f& c, const CvMat* k);

	// x' = undistort(ix - c, k)
	CvPoint2D32f compensateDistortionWithNoOffset(const CvPoint2D32f& ix, const CvPoint2D32f& c, const CvMat* k);

	//================== Functors ====================================>
	
	class NormalizedPointUndistorter {
	public:
		NormalizedPointUndistorter(const CvMat* k) {
			this->k = cvCloneMat(k);
		}
		~NormalizedPointUndistorter() {
			cvReleaseMat(&k);
		}
	public:
		CvPoint2D32f operator() (const CvPoint2D32f& p) {
			return compensateDistortion(p, k);
		}

	private:
		CvMat* k;
	};


	/** undistort an image point
	 * Input the principal point and distortion parameters
	 *
	 */
	class ImagePointUndistorter {
	public:
		ImagePointUndistorter(const CvPoint2D32f cameraCenter, CvMat* k);

		ImagePointUndistorter(
			const CvPoint2D32f cameraCenter,
			double k1,
			double k2 = 0,
			double k3 = 0,
			double k4 = 0);

		~ImagePointUndistorter(void);

		ImagePointUndistorter(const ImagePointUndistorter& rhs);
		ImagePointUndistorter& operator= (const ImagePointUndistorter& rhs);

	public:
		CvPoint2D32f operator() (const CvPoint2D32f& p) const;

		//if offset is true, return undistort(ix - c, k) + c
		//else return undistort(ix - c, k)
		void setOffset(bool v) {
			this->offset = v;
		}

		void setImageCenter(const CvPoint2D32f& cameraCenter);

		void setk(double k1, double k2 = 0, double k3 = 0, double k4 = 0);

	private:
		void init(const CvPoint2D32f& cameraCenter, CvMat* k);

	private:
		CvPoint2D32f cameraCenter;
		CvMat* k;
		bool offset;
	};

}