#pragma once

#include <vector>
#include <iostream>

#include <cv.h>

namespace fvision {

class LookAtParams {
public:
	LookAtParams() {
		location = cvPoint3D64f(0, 0, 0);
		lookat = cvPoint3D64f(0, 0, 1);
		up = cvPoint3D64f(0, 1, 0);
	}

public:
	CvPoint3D64f location;
	CvPoint3D64f lookat;
	CvPoint3D64f up;
};

/**
 * a camera can be represented by P = K[R|t], t = -RC; NP = [R|t]
 * where P is the camera matrix, K is the internal matrix, R is the orientation and C is the camera center
 */
class Camera {
public:
	//K = I
	Camera();

	Camera(const CvMat* K);

	Camera(const CvMat* K, const CvMat* NP);

	Camera(const CvMat* K, const CvMat* R, const CvMat* t);

	//need to do qr decomposition, currently not implemented yet
	static Camera* createWithP(const CvMat* P);

	static Camera* createWithLookAt(const CvMat* K, const LookAtParams& params);

public:

	Camera(const Camera& cam);

	~Camera();

	Camera& operator=(const Camera& rhs);

	void reset(const CvMat* K, const CvMat* R, const CvMat* t);

	void setK(const CvMat* K);

private:
	//initialize data structures
	void initMats();

	void initWithKRt(const CvMat* K, const CvMat* R, const CvMat* t);

	void initWithK(const CvMat* K);

	void initWithKNP(const CvMat* K, const CvMat* NP);

	void initWithCamera(const Camera& cam);

	//deallocate all matrices
	void clear();

public:
	//H 3x3 homography in image space
	//used when image coordinates changed
	//P' = H * P
	//K' = H * K
	void trans(const CvMat* H);
	void transWithScale(double scale);
	void transWithFlipY(double h);

public:
	CvPoint2D32f project(CvPoint3D32f X) const ;

	std::vector<CvPoint2D32f> project(const std::vector<CvPoint3D32f>& Xs) const;

	CvPoint2D32f project(CvPoint3D64f X) const ;

	std::vector<CvPoint2D32f> project(const std::vector<CvPoint3D64f>& Xs) const;

public:

	const CvMat* getP() const { return P;}

	const CvMat* getK() const { return K;}

	const CvMat* getNP() const { return NP; }

	const CvMat* getC() const { return C;}

	const CvMat* getR() const {return R;}

	const CvMat* gett() const { return t;}

	//P = [M|e]
	//M is the first 3x3 matrix of P
	CvMat* getM(CvMat* buf = NULL) const;

	CvPoint3D64f geto() const { return o;}

private:
	CvMat* P;
	CvMat* NP; //normalized camera matrix. NP = [R|t]

	CvMat* K;
	CvMat* R;
	CvMat* C;
	CvMat* t;

	CvPoint3D64f o;
};

std::ostream& operator<<(std::ostream& os, const Camera& cam);
std::istream& operator>>(std::istream& is, Camera& cam);

}