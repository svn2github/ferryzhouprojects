#include "rotation.h"

#include <fvision/utils.h>

namespace fvision {

	CvMat* getZR(double ztheta, CvMat* buf) {

		if (buf == NULL) {
			buf = cvCreateMat(3, 3, CV_64FC1);
		} else {
			assert(buf->rows == 3 && buf->cols == 3);
		}

		cvZero(buf);
		double c = cos(ztheta);
		double s = sin(ztheta);
		cvmSet(buf, 0, 0, c);
		cvmSet(buf, 0, 1, -s);
		cvmSet(buf, 1, 0, s);
		cvmSet(buf, 1, 1, c);
		cvmSet(buf, 2, 2, 1.0);

		return buf;
	}

	CvMat* getXR(double xtheta, CvMat* buf) {

		if (buf == NULL) {
			buf = cvCreateMat(3, 3, CV_64FC1);
		} else {
			assert(buf->rows == 3 && buf->cols == 3);
		}

		cvZero(buf);
		cvmSet(buf, 0, 0, 1.0);

		double c = cos(xtheta);
		double s = sin(xtheta);

		cvmSet(buf, 1, 1, c);
		cvmSet(buf, 1, 2, -s);
		cvmSet(buf, 2, 1, s);
		cvmSet(buf, 2, 2, c);

		return buf;
	}

	CvMat* getYR(double ytheta, CvMat* buf) {

		if (buf == NULL) {
			buf = cvCreateMat(3, 3, CV_64FC1);
		} else {
			assert(buf->rows == 3 && buf->cols == 3);
		}

		cvSetIdentity(buf);

		cvmSet(buf, 0, 0, cos(ytheta));
		cvmSet(buf, 0, 2, sin(ytheta));
		cvmSet(buf, 2, 0, -sin(ytheta));
		cvmSet(buf, 2, 2, cos(ytheta));

		return buf;

/*		double yr[] = {cos(ytheta), 0, sin(ytheta), 
			                 0,     1,      0, 
					   -sin(ytheta), 0, cos(ytheta)};
		CvMat YR = cvMat(3, 3, CV_64FC1, yr);
		return cloneMat(&YR);
*/	}

	double normalizeTheta(double theta) {
		if (theta < -CV_PI / 2) return theta + CV_PI;
		else if (theta > CV_PI / 2) return theta - CV_PI;
		else return theta;
	}

	// xcos(theta) - ysin(theta) = 0
	double getTheta(double x, double y) {
		double theta = atan2(x, y);
		return normalizeTheta(theta);
	}

	//a1 * cos(theta) - a2 * sin(theta) = a1'
	//a1 * sin(theta) + a2 * cos(theta) = a2'
	//if firstToZero, a1' = 0; theta = atan2(a1, a2), if a2' < 0, theta += pi
	//else a2' = 0; theta = atan(a2, -a1), if a1' < 0, theta += pi
	double getThetaWithNextToPositive(double a1, double a2, bool firstToZero) {
		if (firstToZero) {
			double theta = atan2(a1, a2);
			double na2 = a1 * sin(theta) + a2 * cos(theta);
			if (na2 < 0) {
				if (theta < 0) return theta + CV_PI;
				else return theta - CV_PI;
			}
			else return theta;
		} else {
			double theta = atan2(a2, -a1);
			double na1 = a1 * cos(theta) - a2 * sin(theta);
			if (na1 < 0) {
				if (theta < 0) return theta + CV_PI;
				else return theta - CV_PI;
			}
			else return theta;
		}
	}

	CvMat* getYRToYAxis(const CvMat* p) {
		double y_theta = getYThetaToYAxis(p);
		return getYR(y_theta);
	}

	double getYThetaToYAxis(const CvMat* p) {
		return getTheta(cvmGet(p, 0, 0), -cvmGet(p, 2, 0));
	}

	CvMat* getXRToXAxis(const CvMat* p) {
		double x_theta = getTheta(cvmGet(p, 1, 0), cvmGet(p, 2, 0));
		return getXR(x_theta);
	}

	CvMat* getZRToYAxis(const CvMat* p) {
		double z_theta = getTheta(cvmGet(p, 0, 0), cvmGet(p, 1, 0));
		return getZR(z_theta);
	}

	double getZThetaToYAxis(const CvMat* p) {
		return getTheta(cvmGet(p, 0, 0), cvmGet(p, 1, 0));
	}

	CvMat* getZRToXAxis(const CvMat* p) {
		double z_theta = getTheta(cvmGet(p, 1, 0), -cvmGet(p, 0, 0));
		return getZR(z_theta);
	}

	double getZThetaToXAxis(const CvMat* p) {
		return getTheta(cvmGet(p, 1, 0), -cvmGet(p, 0, 0));
	}

	CvMat* getYRToInfinity(const CvMat* p) {
		double y_theta = getTheta(cvmGet(p, 2, 0), cvmGet(p, 0, 0));
		return getYR(y_theta);
	}

	double getYThetaToInfinity(const CvMat* p) {
		return getTheta(cvmGet(p, 2, 0), cvmGet(p, 0, 0));
	}

	CvMat* getXRToInfinity(const CvMat* p) {
		double x_theta = getTheta(cvmGet(p, 2, 0), -cvmGet(p, 1, 0));
		return getXR(x_theta);
	}
	
	double getXThetaToInfinity(const CvMat* p) {
		return getTheta(cvmGet(p, 2, 0), -cvmGet(p, 1, 0));
	}

	CvMat* getXZRToYAxisInfinity(const CvMat* p) {
		CvMat* zr = getZRToYAxis(p);
		CvMat* zrp = matMul(zr, p);
		CvMat* xr = getXRToInfinity(zrp);
		CvMat* r = matMul(xr, zr);
		cvReleaseMat(&zr);
		cvReleaseMat(&zrp);
		cvReleaseMat(&xr);
		return r;
	}

	CvMat* getYZRToXAxisInfinity(const CvMat* p) {
		CvMat* zr = getZRToXAxis(p);
		CvMat* zrp = matMul(zr, p);
		CvMat* yr = getYRToInfinity(zrp);
		CvMat* r = matMul(yr, zr);
		cvReleaseMat(&zr);
		cvReleaseMat(&zrp);
		cvReleaseMat(&yr);
		return r;
	}

	//r: 3x1 matrix
	//q: 4x1 
	void rotationVectorToQuaternion(const CvMat* v, CvMat* q) {
		double theta = cvNorm(v);
		double s = sin(theta / 2) / theta;
		cvmSet(q, 0, 0, s * cvmGet(v, 0, 0));
		cvmSet(q, 1, 0, s * cvmGet(v, 1, 0));
		cvmSet(q, 2, 0, s * cvmGet(v, 2, 0));
		cvmSet(q, 3, 0, cos(theta / 2));
	}

	void rotationVectorFromQuaternion(CvMat* v, const CvMat* q) {
		cvGetSubRect(q, v, cvRect(0, 0, 1, 3));
		double s = cvNorm(v);
		double theta = atan2(s, cvmGet(q, 3, 0)) * 2;
		double scale = theta / sin(theta / 2);
		cvScale(v, v, scale);
	}

	void rotationMatrixToRotationVector(const CvMat* m, CvMat* v) {
		cvRodrigues2(m, v);
	}

	void rotationMatrixFromRotationVector(CvMat* m, const CvMat* v) {
		cvRodrigues2(v, m);
	}

	void rotationMatrixToQuaternion(const CvMat* m, CvMat* q) {
		CvMat* v = cvCreateMat(3, 1, CV_64FC1);
		rotationMatrixToRotationVector(m, v);
		rotationVectorToQuaternion(v, q);
		cvReleaseMat(&v);
	}

	void rotationMatrixFromQuaternion(CvMat* m, const CvMat* q) {
		CvMat* v = cvCreateMat(3, 1, CV_64FC1);
		rotationVectorFromQuaternion(v, q);
		rotationMatrixFromRotationVector(m, v);
		cvReleaseMat(&v);
	}

	//(Rx*Ry*Rz)*R = I -> R = R(-z) * R(-y) * R(-x)
	//Rz * R => R(1, 0) = 0
	//Ry * R' => R'(2, 0) = 0; R' = Rz * R;
	//Rx * R' => R''(2, 1) = 0; R'' = Ry * R'
	void decomposeR(CvMat* R, double& oxtheta, double& oytheta, double& oztheta) {
		//CvMat* nR = cvCloneMat(R);
		//if (cvmGet(R, 0, 0) < 0) cvScale(nR, nR, -1);
		double ztheta = getThetaWithNextToPositive(cvmGet(R, 0, 0), cvmGet(R, 1, 0), false);
		CvMat* Rz = getZR(ztheta);
		CvMat* RzR = matMul(Rz, R);
		double ytheta = -getThetaWithNextToPositive(cvmGet(RzR, 0, 0), cvmGet(RzR, 2, 0), false);
		CvMat* Ry = getYR(ytheta);
		CvMat* RyRzR = matMul(Ry, RzR);
		double xtheta = getThetaWithNextToPositive(cvmGet(RyRzR, 1, 1), cvmGet(RyRzR, 2, 1), false);
		CvMat* Rx = getXR(xtheta);
		CvMat* RxRyRzR = matMul(Rx, RyRzR);
		//cout<<"RzR"<<RzR<<endl;
		//cout<<"RyRzR"<<RyRzR<<endl;
		//cout<<"RxRyRzR"<<RxRyRzR<<endl;

		oxtheta = -xtheta;
		oytheta = -ytheta;
		oztheta = -ztheta;

		//cvReleaseMat(&nR);
		cvReleaseMat(&Rz);
		cvReleaseMat(&RzR);
		cvReleaseMat(&Ry);
		cvReleaseMat(&RyRzR);
		cvReleaseMat(&RxRyRzR);
	}

	CvMat* composeRzyx(double xtheta, double ytheta, double ztheta, CvMat* buf) {
		if (buf == NULL) buf = cvCreateMat(3, 3, CV_64FC1);
		CvMat* XR = getXR(xtheta);
		CvMat* YR = getYR(ytheta);
		CvMat* YXR = matMul(YR, XR);
		CvMat* ZR = getZR(ztheta);
		cvMatMul(ZR, YXR, buf);
		clearMats(&XR, &YR, &YXR, &ZR);
		return buf;
	}

}