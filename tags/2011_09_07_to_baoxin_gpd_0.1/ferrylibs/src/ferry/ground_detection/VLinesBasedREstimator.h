#pragma once

#include <algorithm>
#include <iostream>

#include <ferry/cv_geometry/CvMatUtil.h>

using namespace std;

using namespace ferry::cv_mat;

//ztheta -> x = 0
//xtheta -> z = 0
class ToVerticalR {
public:
	ToVerticalR(CvMat* vp) {
		double x0 = cvmGet(vp, 0, 0);
		double y0 = cvmGet(vp, 1, 0);
		double z0 = cvmGet(vp, 2, 0);
		
		ztheta = -atan(x0/y0);
		double y1 = -sin(ztheta) * x0 + cos(ztheta) * y0;

		xtheta = atan(z0 / y1);

		cout<<"n vp: "<<matMul(getR(), vp)<<endl;
	}

	ToVerticalR(float xtheta, float ztheta) {
		this->xtheta = xtheta;
		this->ztheta = ztheta;
	}

	CvMat* getR() {
		double zr[] = {cos(ztheta), sin(ztheta), 0,  -sin(ztheta), cos(ztheta), 0,  0, 0, 1};
		CvMat ZR = cvMat(3, 3, CV_64FC1, zr);
		double xr[] = {1, 0, 0, 0, cos(xtheta), sin(xtheta), 0, -sin(xtheta), cos(xtheta)};
		CvMat XR = cvMat(3, 3, CV_64FC1, xr);

		return matMul(&XR, &ZR);
	}

	static CvMat* getZR(float ztheta) {
		double zr[] = {cos(ztheta), sin(ztheta), 0,  -sin(ztheta), cos(ztheta), 0,  0, 0, 1};
		CvMat ZR = cvMat(3, 3, CV_64FC1, zr);
		return cloneMat(&ZR);
	}

	static CvMat* getXR(float xtheta) {
		double xr[] = {1, 0, 0, 0, cos(xtheta), sin(xtheta), 0, -sin(xtheta), cos(xtheta)};
		CvMat XR = cvMat(3, 3, CV_64FC1, xr);
		return cloneMat(&XR);
	}

public:
	float xtheta;
	float ztheta;
};

/**
 * K, lines
 * lines -> normalized. nline = Kt * line
 * nlines -> vanishing points -> theta, vy
 * theta,vy -> error estimation -> best theta, vy -> best R
 */
class VLinesBasedREstimator
{
public:
	VLinesBasedREstimator(void) {

	}

	~VLinesBasedREstimator(void) {}

public:
	CvMat* compute(CvMat* K, vector<CvMat*> lines) {
		ToVerticalR vr = computeVR(K, lines);
		return vr.getR();
	}

	ToVerticalR computeVR(CvMat* K, vector<CvMat*> lines) {
		CvMat* Kt = transpose(K); cout<<"Kt: "<<Kt<<endl;
		
		//normalize
		vector<CvMat*> nlines;
		for (int i = 0; i < lines.size(); i++) {
			cout<<"line: "<<lines[i]<<endl;
			cout<<"nline: "<<matMul(Kt, lines[i])<<endl;
			nlines.push_back(matMul(Kt, lines[i]));
		}

		vector<CvMat*> crossPoints = getAllCrossPoints(nlines);

		vector<ToVerticalR> vrs = estimateVRs(crossPoints);

		vector<CvMat*> rs = vrsToMats(vrs);

		vector<double> errors = estimateErrors(rs, nlines);

		int index = min_element(errors.begin(), errors.end()) - errors.begin();

		//return vrs[index];

		/////
		float xtheta0 = vrs[index].xtheta;
		float ztheta0 = vrs[index].ztheta;
		double minError = errors[index];

		const int RANGE_N = 5;
		const double PRECISION_THETA = CV_PI / 180 * 0.05;

		ToVerticalR bestVR(xtheta0, ztheta0);
		for (int i = -RANGE_N; i <= RANGE_N; i++) {
			for (int j = -RANGE_N; j <= RANGE_N; j++) {
				ToVerticalR vr(xtheta0 + i * PRECISION_THETA, ztheta0 + j * PRECISION_THETA);
				double error = estimateErrors(vr.getR(), nlines);
				if (error < minError) {
					bestVR = vr;
					minError = error;
				}
			}
		}

		cout<<"initial min error: "<<errors[index]<<endl;
		cout<<"min error: "<<minError<<endl;

		return bestVR;
	}

public:
	vector<CvMat*> getAllCrossPoints(vector<CvMat*>& nlines) {
		vector<CvMat*> pts;
		for (int i = 0; i < nlines.size() - 1; i++) {
			cout<<"line: "<<i<<" "<<nlines[i]<<endl;
			for (int j = i + 1; j < nlines.size(); j++) {
				CvMat* pt = cvCreateMat(3, 1, CV_64FC1);
				cvCrossProduct(nlines[i], nlines[j], pt);
				pts.push_back(pt);
			}
		}
		return pts;
	}

	vector<ToVerticalR> estimateVRs(vector<CvMat*>& pts) {
		vector<ToVerticalR> vrs;
		for (int i = 0; i < pts.size(); i++) {
			vrs.push_back(ToVerticalR(pts[i]));
		}
		return vrs;
	}

	vector<CvMat*> vrsToMats(vector<ToVerticalR>& vrs) {
		vector<CvMat*> rs;
		for (int i = 0; i < vrs.size(); i++) {
			rs.push_back(vrs[i].getR());
		}
		return rs;
	}

	static double estimateError(CvMat* R, CvMat* nline) {
		CvMat* newline = matMul(R, nline);

		cout<<"nline: "<<nline<<endl;
		cout<<"newline: "<<newline<<endl;

		double e = abs(cvmGet(newline, 1, 0) / cvmGet(newline, 0, 0));

		cvReleaseMat(&newline);

		return e;
	}

	static double estimateErrors(CvMat* R, vector<CvMat*> nlines) {
		double es = 0;
		for (int i = 0; i < nlines.size(); i++) {
			es += estimateError(R, nlines[i]);
		}
		return es;
	}

	vector<double> estimateErrors(vector<CvMat*> rs, vector<CvMat*> nlines) {
		vector<double> ess;
		for (int i = 0; i < rs.size(); i++) {
			ess.push_back(estimateErrors(rs[i], nlines));
		}
		return ess;
	}
private:
	CvMat* K;
	vector<CvMat*> lines;

	CvMat* Kt;

	CvMat* R;
};
