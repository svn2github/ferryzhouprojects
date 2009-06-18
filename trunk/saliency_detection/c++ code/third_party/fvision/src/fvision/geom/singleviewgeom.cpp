#include "singleviewgeom.h"

#include "camera_utils.h"

#include <fvision/utils/point_mat_conversion_template.h>
#include <fvision/utils/geom.h>
#include <fvision/utils/cvmat_utils.h>

#include <iostream>

using namespace std;

namespace fvision {

	CvPoint2D32f project(const CvPoint3D64f& camPosition, const CvMat* R, const CvPoint3D64f& p) {
		CvPoint3D64f dp = p - camPosition;
		CvPoint3D64f rdp = rotatePoint(dp, R); 
		return cvPoint2D32f(rdp.x / rdp.z, rdp.y / rdp.z);
	}

	CvPoint2D32f project(const CvMat* M, const CvPoint3D32f& p3d) {
		CvMat* mp2d = matMul(M, matFromPoint3D(p3d));
		CvPoint2D32f p2d;
		hmatToPoint2D(mp2d, p2d);
		cvReleaseMat(&mp2d);
		return p2d;
	}

	vector<CvPoint2D32f> project(const CvMat* M, const vector<CvPoint3D32f>& p3ds) {
		vector<CvPoint2D32f> p2ds;
		for (size_t i = 0; i < p3ds.size(); i++) {
			p2ds.push_back(project(M, p3ds[i]));
		}
		return p2ds;
	}

	CvPoint2D32f projectWithP(const CvMat* P, const CvPoint3D32f& p3d) {
	/*	CvMat* hmat = hmatFromPoint3D(p3d);
		CvMat* mp2d = matMul(P, hmat);
		CvPoint2D32f p2d = hmatToPoint2D(mp2d);
		cvReleaseMat(&mp2d);
		cvReleaseMat(&hmat);
		return p2d;
	*/
		double p00 = cvmGet(P, 0, 0);
		double p01 = cvmGet(P, 0, 1);
		double p02 = cvmGet(P, 0, 2);
		double p03 = cvmGet(P, 0, 3);
		double p10 = cvmGet(P, 1, 0);
		double p11 = cvmGet(P, 1, 1);
		double p12 = cvmGet(P, 1, 2);
		double p13 = cvmGet(P, 1, 3);
		double p20 = cvmGet(P, 2, 0);
		double p21 = cvmGet(P, 2, 1);
		double p22 = cvmGet(P, 2, 2);
		double p23 = cvmGet(P, 2, 3);

		double x = p00 * p3d.x + p01 * p3d.y + p02 * p3d.z + p03;
		double y = p10 * p3d.x + p11 * p3d.y + p12 * p3d.z + p13;
		double w = p20 * p3d.x + p21 * p3d.y + p22 * p3d.z + p23;

		return cvPoint2D32f(x / w, y / w);
	}

	vector<CvPoint2D32f> projectWithP(const CvMat* P, const vector<CvPoint3D32f>& p3ds) {
		vector<CvPoint2D32f> p2ds;
		for (size_t i = 0; i < p3ds.size(); i++) {
			p2ds.push_back(projectWithP(P, p3ds[i]));
		}
		return p2ds;
	}

	CvPoint2D32f projectWithP(const CvMat* P, const CvPoint3D64f& p3d) {
	/*	CvMat* hmat = hmatFromPoint3D(p3d);
		CvMat* mp2d = matMul(P, hmat);
		CvPoint2D32f p2d = hmatToPoint2D(mp2d);
		cvReleaseMat(&mp2d);
		cvReleaseMat(&hmat);
		return p2d;
	*/
		double p00 = cvmGet(P, 0, 0);
		double p01 = cvmGet(P, 0, 1);
		double p02 = cvmGet(P, 0, 2);
		double p03 = cvmGet(P, 0, 3);
		double p10 = cvmGet(P, 1, 0);
		double p11 = cvmGet(P, 1, 1);
		double p12 = cvmGet(P, 1, 2);
		double p13 = cvmGet(P, 1, 3);
		double p20 = cvmGet(P, 2, 0);
		double p21 = cvmGet(P, 2, 1);
		double p22 = cvmGet(P, 2, 2);
		double p23 = cvmGet(P, 2, 3);

		double x = p00 * p3d.x + p01 * p3d.y + p02 * p3d.z + p03;
		double y = p10 * p3d.x + p11 * p3d.y + p12 * p3d.z + p13;
		double w = p20 * p3d.x + p21 * p3d.y + p22 * p3d.z + p23;

		return cvPoint2D32f(x / w, y / w);
	}

	vector<CvPoint2D32f> projectWithP(const CvMat* P, const vector<CvPoint3D64f>& p3ds) {
		vector<CvPoint2D32f> p2ds;
		for (size_t i = 0; i < p3ds.size(); i++) {
			p2ds.push_back(projectWithP(P, p3ds[i]));
		}
		return p2ds;
	}

	//p is in image coordinate system. in which the (0, 0) is the left-bottom point
	CvPoint3D32f backProjectWithDepth(const CvMat* P, CvPoint2D32f p, double depth) {
		double c0, c1, c2;
		double x, y, z = depth;
		double u = p.x;
		double v = p.y;

		c0 = z * cvmGet(P, 0, 2) + cvmGet(P, 0, 3);
		c1 = z * cvmGet(P, 1, 2) + cvmGet(P, 1, 3);
		c2 = z * cvmGet(P, 2, 2) + cvmGet(P, 2, 3);

		y = u * (c1 * cvmGet(P, 2, 0) - cvmGet(P, 1, 0) * c2) +
			v * (c2 * cvmGet(P, 0, 0) - cvmGet(P, 2, 0) * c0) +
			cvmGet(P, 1, 0) * c0 - c1 * cvmGet(P, 0, 0);

		y /= v * (cvmGet(P, 2, 0) * cvmGet(P, 0, 1) - cvmGet(P, 2, 1) * cvmGet(P, 0, 0)) +
			 u * (cvmGet(P, 1, 0) * cvmGet(P, 2, 1) - cvmGet(P, 1, 1) * cvmGet(P, 2, 0)) +
			cvmGet(P, 0, 0) * cvmGet(P, 1, 1) - cvmGet(P, 1, 0) * cvmGet(P, 0, 1);
			
		x = y * (cvmGet(P, 0, 1) - cvmGet(P, 2, 1) * u) + c0 - c2 * u;
		x /= cvmGet(P, 2, 0) * u - cvmGet(P, 0, 0);

		return cvPoint3D32f(x, y, z);
	}

	CvMat* getHFromRt(CvMat* R, CvMat* t) {

		CvMat* H = cvCreateMat(4, 4, CV_64FC1);
		cvZero(H);
		setSubRect(H, R, cvRect(0, 0, 3, 3));
		setSubRect(H, t, cvRect(3, 0, 1, 3));
		cvmSet(H, 3, 3, 1);

		return H;
	}

	CvMat* getHFromP(CvMat* P) {

		CvMat* H = cvCreateMat(4, 4, CV_64FC1);
		cvZero(H);
		setSubRect(H, P, cvRect(0, 0, 4, 3));
		cvmSet(H, 3, 3, 1);

		return H;
	}

	CvMat* getPNormalizationH(CvMat* P) {

		CvMat* invH = getHFromP(P);
		CvMat* H = inv(invH);

		cvReleaseMat(&invH);
/*
		CvMat* M = cvCreateMat(3, 3, CV_64FC1);
		CvMat* t = cvCreateMat(3, 1, CV_64FC1);
		cvGetSubRect(P, M, cvRect(0, 0, 3, 3));
		cvGetSubRect(P, t, cvRect(3, 0, 1, 3));

		CvMat* invM = inv(M);
		CvMat* invMt = matMul(invM, t);
		cvScale(invMt, invMt, -1);

		CvMat* H = getHFromRt(invM, invMt);

		cvReleaseMat(&M);
		cvReleaseMat(&t);
		cvReleaseMat(&invM);
		cvReleaseMat(&invMt);
*/
		return H;
	}




}