#pragma once

#include <iostream>
#include <ferry/cv_geometry/DLT_HC.h>

using namespace std;
using namespace ferry::cv_geometry;

namespace ferry {
	namespace cv_geometry {
		namespace test {

class Test_DLT_HC
{
public:
	void test() {
		vector<CvPoint2D32f> x1s, x2s;

		x1s.push_back(cvPoint2D32f(2.11, 4.14));
		x1s.push_back(cvPoint2D32f(2.22, 3.64));
		x1s.push_back(cvPoint2D32f(3.00, 3.66));
		x1s.push_back(cvPoint2D32f(2.98, 4.17));

		x2s.push_back(cvPoint2D32f(0, 1));
		x2s.push_back(cvPoint2D32f(0, 0));
		x2s.push_back(cvPoint2D32f(1, 0));
		x2s.push_back(cvPoint2D32f(1, 1));

		DLT_HC hc;
		Homography H = hc.compute(x1s, x2s);
		cout<<H.toString()<<endl;

		for (int i = 0; i < x1s.size(); i++) {
			CvPoint2D32f tp1 = H.transform(x1s[i]);
			cout<<tp1.x<<", "<<tp1.y<<endl;
			cout<<H.transfromDistance(x1s[i], x2s[i])<<endl;
		}
	}
};

		}
	}
}