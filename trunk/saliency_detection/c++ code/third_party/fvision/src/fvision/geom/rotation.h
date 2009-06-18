#pragma once

#include <cv.h>

/*!
rotation matrix utils
estimate rotation matrix by transform points

*/
namespace fvision {
	//[ cos(ztheta), -sin(ztheta), 0;
	//  sin(ztheta),  cos(ztheta), 0;
	//       0,             0,     1]
	CvMat* getZR(double ztheta, CvMat* buf = NULL);

	CvMat* getXR(double xtheta, CvMat* buf = NULL);

	CvMat* getYR(double ytheta, CvMat* buf = NULL);

	// normalize theta so that it is in [-pi/2, pi/2]
	double normalizeTheta(double theta);

	// xcos(theta) - ysin(theta) = 0
	// normalize theta so that it is in [-pi/2, pi/2]
	double getTheta(double x, double y);

	//a1 * cos(theta) - a2 * sin(theta) = a1'
	//a1 * sin(theta) + a2 * cos(theta) = a2'
	//if firstToZero, a1' = 0; theta = atan2(a1, a2), if a2' < 0, theta += pi
	//else a2' = 0; theta = atan(a2, -a1), if a1' < 0, theta += pi
	double getThetaWithNextToPositive(double a1, double a2, bool firstToZero);

	// rotate around y axis so that the point is transformed to y axis
	// np = YR * p so that np.x = 0
	// cos(theta) * p.x + sin(theta) * p.z = 0
	CvMat* getYRToYAxis(const CvMat* p);
	double getYThetaToYAxis(const CvMat* p);

	// rotate around x axis so that the point is transformed to x axis
	// np = XR * p so that np.y = 0
	// cos(theta) * p.y - sin(theta) * p.z = 0
	CvMat* getXRToXAxis(const CvMat* p);

	// rotate around z axis so that the point is transformed to y axis
	// np = ZR * p so that np.x = 0
	// cos(theta) * p.x - sin(theta) * p.y = 0
	CvMat* getZRToYAxis(const CvMat* p);
	double getZThetaToYAxis(const CvMat* p);

	// rotate around z axis so that the point is transformed to x axis
	// np = ZR * p so that np.y = 0
	// sin(theta) * p.x + cos(theta) * p.y = 0
	CvMat* getZRToXAxis(const CvMat* p);
	double getZThetaToXAxis(const CvMat* p);

	// rotate around y axis so that the point is transformed to infinity
	// np = YR * p so that np.z = 0
	// -sin(theta) * p.x + cos(theta) * p.z = 0
	CvMat* getYRToInfinity(const CvMat* p);
	double getYThetaToInfinity(const CvMat* p);

	// rotate around x axis so that the point is transformed to infinity
	// np = XR * p so that np.z = 0
	// sin(theta) * p.y + cos(theta) * p.z = 0
	CvMat* getXRToInfinity(const CvMat* p);
	double getXThetaToInfinity(const CvMat* p);

	//First rotate around z axis so that the point is transformed to y axis
	//then rotate around x axis so that the point is transformed to infinity
	//return xr * zr
	CvMat* getXZRToYAxisInfinity(const CvMat* p);

	//transform point to (1, 0, 0)T
	//used for stereo rectification
	//First rotate around z axis so that the point is transformed to x axis
	//then rotate around y axis so that the point is transformed to infinity
	//return yr * zr
	CvMat* getYZRToXAxisInfinity(const CvMat* p);

	/** convert rotation vector to quaternion
	 * q = (vn * sin(theta / 2), cos(theta / 2))
	 * v = vn * theta
	 * 
	 * @param v rotation vector, a 3x1 matrix
	 * @param q quaterion, a 4x1 matrix
	 */
	void rotationVectorToQuaternion(const CvMat* v, CvMat* q);

	/** convert quaternion to rotation vector 
	 * 
	 * @param v rotation vector, a 3x1 matrix
	 * @param q quaterion, a 4x1 matrix
	 */
	void rotationVectorFromQuaternion(CvMat* v, const CvMat* q);


	/** convert rotation matrix to rotation vector
	 *
	 * @param m rotation matrix, a 3x3 matrix
	 * @param v rotation vector, a 3x1 matrix
	 */
	void rotationMatrixToRotationVector(const CvMat* m, CvMat* v);


	/** convert rotation vector to rotation matrix
	 *
	 * @param m rotation matrix, a 3x3 matrix
	 * @param v rotation vector, a 3x1 matrix
	 */
	void rotationMatrixFromRotationVector(CvMat* m, const CvMat* v);

	/** convert rotation matrix to quaternion
	 *
	 * @param m rotation matrix, a 3x3 matrix
	 * @param q quaterion, a 4x1 matrix
	 */
	void rotationMatrixToQuaternion(const CvMat* m, CvMat* q);

	/** convert quaternion to rotation matrix
	 *
	 * @param m rotation vector, a 3x3 matrix
	 * @param q quaterion, a 4x1 matrix
	 */
	void rotationMatrixFromQuaternion(CvMat* m, const CvMat* q);


	//R = R(ztheta) * R(ytheta) * R(xtheta)
	void decomposeR(CvMat* R, double& xtheta, double& ytheta, double& ztheta);

	//R = R(ztheta) * R(ytheta) * R(xtheta)
	CvMat* composeRzyx(double xtheta, double ytheta, double ztheta, CvMat* buf = NULL);
}