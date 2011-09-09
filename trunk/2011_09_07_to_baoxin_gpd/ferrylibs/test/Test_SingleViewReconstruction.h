#pragma once

#include <iostream>
#include <ferry/cv_geometry/TwoViewSimulation.h>
#include <ferry/cv_geometry/TriangleMesh.h>
#include <ferry/cv_geometry/DrawingUtil.h>

using namespace std;
using namespace ferry::cv_geometry;

namespace ferry {
	namespace cv_geometry {
		namespace test {

class Test_SingleViewReconstruction {
public:
	Test_SingleViewReconstruction(void) {
	}
	~Test_SingleViewReconstruction(void) {
	}

public:
	void test() {
		vector<CvPoint3D32f> sameHeightPoints;
		float height = -2;
		double r = 1;
		for (int i = 0; i < 50; i++) {
			double z = height;
			double x = r * sin(i * 3.14 / 180 * 5) * 2;
			double y = r * cos(i * 3.14 / 180 * 5);
			sameHeightPoints.push_back(cvPoint3D32f(x, y, z));
			sameHeightPoints.push_back(cvPoint3D32f(x, y, z + 1));
		}

		Camera cam1(3, cvPoint3D32f(-5, -5, 0), cvPoint3D32f(1, 1, 0), cvPoint3D32f(0, 0, 1));

		vector<CvPoint2D32f> imagePoints = cam1.calcImagePoints(sameHeightPoints);

		cout<<"imagePoints:"<<endl;
		cout<<imagePoints<<endl;

		int width = 400;
		IplImage* im = cvCreateImage(cvSize(width, width), 8, 3);
		cvZero(im);
		
		vector<CvPoint> rps = toRealImagePoints(imagePoints, width);
		drawCrosses(im, rps, CV_RGB(0, 255, 0));

		cvNamedWindow("im", 1);
		cvShowImage("im", im);

		CvMat* M = matMul(cam1.getK(), cam1.getR());
		CvMat* iM = inv(M);

		vector<CvMat*> m_TXs;
		vector<CvPoint2D32f> ps;
		CvMat* C = cam1.getC();
		height = -1.5;
		for (int i = 0; i < imagePoints.size(); i++) {
			CvMat* m_p = hmatFromPoint2D(imagePoints[i]);
			CvMat* m_TX = matMul(iM, m_p);
			m_TXs.push_back(m_TX);
			cout<<cvmGet(m_TX, 0, 0)<<", "<<cvmGet(m_TX, 1, 0)<<", "<<cvmGet(m_TX, 2, 0)<<", "<<endl;

			double x = cvmGet(m_TX, 0, 0);
			double y = cvmGet(m_TX, 1, 0);
			double z = cvmGet(m_TX, 2, 0);

			x = x / z * height;
			y = y / z * height;
			z = z / z * height;

			cout<<x<<", "<<y<<", "<<z<<endl;
			cout<<" = "<<sameHeightPoints[i].x + 5<<", "<<sameHeightPoints[i].y + 5<<", "<<sameHeightPoints[i].z<<endl;

			ps.push_back(cvPoint2D32f(x, y));
		}

		//vector<CvPoint2D32f> ps = vectorHmatToPoint2D(m_TXs);

		cout<<ps<<endl;
		
		//int width = 400;
		IplImage* im2 = cvCreateImage(cvSize(width, width), 8, 3);
		cvZero(im2);
		
		vector<CvPoint> rps2 = toRealImagePoints(ps, width, 0.15);
		drawCrosses(im2, rps2, CV_RGB(0, 255, 0));

		cvNamedWindow("im2", 1);
		cvShowImage("im2", im2);

		cout<<"C: "<<C<<endl;
		CvPoint3D32f X1 = sameHeightPoints[0];
		CvPoint2D32f x1 = imagePoints[0];

		CvMat* rX1 = sub(matFromPoint3D(X1), cam1.getC());

		CvMat* iX1 = matMul(M, rX1);

		cout<<"iX1:"<<iX1<<endl;

		CvPoint2D32f piX1 = hmatToPoint2D(iX1);

		cout<<"piX1: "<<piX1<<endl;
		cout<<"x1: "<<x1<<endl;

		CvMat* mx1 = hmatFromPoint2D(x1);
		CvMat* imx1 = matMul(inv(M), mx1);
		cout<<"imx1:"<<imx1<<endl;
		cout<<"rX1: "<<rX1<<endl;

		for (int i = 0; i < 3; i++) cout<<cvmGet(imx1, i, 0) / cvmGet(rX1, i, 0)<<endl;

	}

	vector<CvPoint> toRealImagePoints(vector<CvPoint2D32f>& ps, int width, double scale = 1) {
		vector<CvPoint> nps;
		for (int i = 0; i < ps.size(); i++) {
			CvPoint p = cvPoint((int)(width / 2 * (1 + ps[i].x * scale)), (int)(width / 2 * ( 1 - ps[i].y * scale)));
			nps.push_back(p);
		}
		return nps;
	}
};

		}
	}
}