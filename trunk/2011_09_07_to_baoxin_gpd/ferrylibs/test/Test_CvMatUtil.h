#pragma once

#include <iostream>
#include <ferry/cv_geometry/CvMatUtil.h>

using namespace std;
using namespace ferry::cv_mat;

namespace ferry {
	namespace cv_mat {
		namespace test {

class Test_CvMatUtil
{
public:
	void test() {
		testPseudoinverse();
	}

	void testPseudoinverse() {
		double d[] = {1, 2, 3, 4,
			          5, 3, 1, 6,
					  2, 5, 2, 7};
		CvMat P = cvMat(3, 4, CV_64FC1, d);
		doTestPseudoinverse(&P);
		
		CvMat P2 = cvMat(4, 3, CV_64FC1, d);
		doTestPseudoinverse(&P2);
	}

	void doTestPseudoinverse(CvMat* P) {
		CvMat* PP = CvMatUtil::pseudoinverse(P);
		CvMat* PtPP = cvCreateMat(P->rows, P->rows, CV_64FC1);
		cvMatMul(P, PP, PtPP);
		
		cout<<"P: "<<CvMatUtil::matString(P)<<endl;
		cout<<"PP: "<<CvMatUtil::matString(PP)<<endl;
		cout<<"P * PP: "<<CvMatUtil::matString(PtPP)<<endl;


		CvMat* PI = pi(P);
		CvMat* PPI = CvMatUtil::matMul(P, PI);
		cout<<"PI: "<<CvMatUtil::matString(PI)<<endl;
		cout<<"P * PI: "<<CvMatUtil::matString(PPI)<<endl;
	}

	CvMat* pi(CvMat* P) {
		if (P->rows < P->cols) {
			CvMat* P1 = cvCreateMat(P->cols, P->cols, P->type);
			CvMat* P1I = cvCreateMat(P->cols, P->cols, P->type);
			cvZero(P1);
			CvMatUtil::setSubRect(P1, P, cvRect(0, 0, P->cols, P->rows));
			cvInvert(P1, P1I, CV_SVD);
			cout<<"P1I: "<<CvMatUtil::matString(P1I)<<endl;
			cout<<"P1 * P1I: "<<CvMatUtil::matString(CvMatUtil::matMul(P1, P1I))<<endl;
			CvMat* PI = cvCreateMat(P->cols, P->rows, P->type);
			cvGetCols(P1I, PI, 0, P->rows);
			return PI;
		} else if (P->rows > P->cols) {
			CvMat* P1 = cvCreateMat(P->rows, P->rows, P->type);
			CvMat* P1I = cvCreateMat(P->rows, P->rows, P->type);
			cvZero(P1);
			CvMatUtil::setSubRect(P1, P, cvRect(0, 0, P->cols, P->rows));
			cvInvert(P1, P1I, CV_SVD);
			cout<<"P1I: "<<CvMatUtil::matString(P1I)<<endl;
			cout<<"P1 * P1I: "<<CvMatUtil::matString(CvMatUtil::matMul(P1, P1I))<<endl;
			CvMat* PI = cvCreateMat(P->cols, P->rows, P->type);
			cvGetRows(P1I, PI, 0, P->cols);
			return PI;
		} else return P;
	}

};

		}
	}
}