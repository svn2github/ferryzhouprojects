#pragma once

#include <iostream>
#include <ferry/cv_geometry/Camera.h>

using namespace std;
using namespace ferry::cv_geometry;

namespace ferry {
	namespace cv_geometry {
		namespace test {

class Test_Camera
{
public:
	void test() {
		double f = 3.0;
		CvPoint3D32f o = cvPoint3D32f(-5, -5, 0);
		CvPoint3D32f v = cvPoint3D32f(1, 1, 0);
		CvPoint3D32f u = cvPoint3D32f(0, 0, 1);
		Camera cam(f, o, v, u);
	}

};

		}
	}
}