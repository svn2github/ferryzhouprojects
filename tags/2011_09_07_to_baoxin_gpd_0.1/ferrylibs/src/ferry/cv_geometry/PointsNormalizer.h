#pragma once

#include <cassert>
#include "CvMatUtil.h"

using namespace ferry::cv_mat;

/**
 * Normalized the points. 
 * 1) make the center to the origin. by translation
 * 2) scale to make the mean distance to be sqrt(2.0). 
 * 
 * nX = T * X
 * T = [w, 0, -w*mx; 0, w, -w*my; 0, 0, 1]
 * mx = sum(x)/n; my = sum(y)/n
 * w = sqrt(2.0) / (sqrt(sum((x-mx)^2 + (y-my)^2))/n)
 */
namespace ferry {
	namespace cv_geometry {

class PointsNormalizer
{
public:

	static CvMat* normalize(const vector<CvPoint2D32f>& xs, vector<CvPoint2D32f>& nxs) {
		
		double mx = 0;
		double my = 0;
		int n = (int)xs.size();
		int i = 0;
		for (i = 0; i < n; i++) {
			mx += xs[i].x;
			my += xs[i].y;
		}
		mx /= n;
		my /= n;
		for (i = 0; i < n; i++) {
			CvPoint2D32f p;
			p.x = xs[i].x - mx;
			p.y = xs[i].y - my;
			nxs.push_back(p);
		}

		double w = 0;
		double d = 0;
		for (i = 0; i < n; i++) {
			d += sqrt(nxs[i].x * nxs[i].x + nxs[i].y * nxs[i].y);
		}
		d /= n;
		w = sqrt(2.0) / d;
		for (i = 0; i < n; i++) {
			nxs[i].x *= w;
			nxs[i].y *= w;
		}

		//T = [w, 0, -w*mx; 0, w, -w*my; 0, 0, 1]
		CvMat* T = cvCreateMat(3, 3, CV_64FC1);
		cvZero(T);
		cvmSet(T, 0, 0, w);
		cvmSet(T, 1, 1, w);
		cvmSet(T, 0, 2, -w * mx);
		cvmSet(T, 1, 2, -w * my);
		cvmSet(T, 2, 2, 1);

		return T;
	}
};

	}
}