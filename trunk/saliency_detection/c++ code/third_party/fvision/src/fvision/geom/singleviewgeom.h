#pragma once

#include <cv.h>

#include <vector>

namespace fvision {

	/// x = R * (p - c)
	CvPoint2D32f project(const CvPoint3D64f& camPosition, const CvMat* R, const CvPoint3D64f& p);

	//M = KR, 3x3
	CvPoint2D32f project(const CvMat* M, const CvPoint3D32f& p3d);
	std::vector<CvPoint2D32f> project(const CvMat* M, const std::vector<CvPoint3D32f>& p3ds);

	//P = KR[I|t] 3 x 4
	CvPoint2D32f projectWithP(const CvMat* P, const CvPoint3D32f& p3d);
	std::vector<CvPoint2D32f> projectWithP(const CvMat* P, const std::vector<CvPoint3D32f>& p3ds);

	CvPoint2D32f projectWithP(const CvMat* P, const CvPoint3D64f& p3d);
	std::vector<CvPoint2D32f> projectWithP(const CvMat* P, const std::vector<CvPoint3D64f>& p3ds);

	CvPoint3D32f backProjectWithDepth(const CvMat* P, CvPoint2D32f p, double depth);

	//P' = P * H = [I|0]
	//H = inv([P; 0, 1])
	//P = [M|t]
	//H = [inv(M), -inv(M) * t; 0, 1]
	CvMat* getPNormalizationH(CvMat* P);

	//H = [R, t; 0, 1] 4x4
	//R: 3x3, t: 3x1
	CvMat* getHFromRt(CvMat* R, CvMat* t);

	//H = [P; 0, 1] 4x4
	//P: 3 x 4
	CvMat* getHFromP(CvMat* P);




}
