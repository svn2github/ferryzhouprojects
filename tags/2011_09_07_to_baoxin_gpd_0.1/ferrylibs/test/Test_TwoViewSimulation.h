#pragma once

#include <iostream>
#include <ferry/cv_geometry/TwoViewSimulation.h>
#include <ferry/cv_geometry/TriangleMesh.h>

using namespace std;
using namespace ferry::cv_geometry;

namespace ferry {
	namespace cv_geometry {
		namespace test {

class Test_TwoViewSimulation
{
public:
	void test() {
		TriangleMesh tm;
		tm.readOffFile("data/off_files/cube.off");

		Camera cam1(3, cvPoint3D32f(-5, -5, 0), cvPoint3D32f(1, 1, 0), cvPoint3D32f(0, 0, 1));
		Camera cam2(2, cvPoint3D32f(5, -5, 0), cvPoint3D32f(-1, 1, 0), cvPoint3D32f(0, 0, 1));
		
		TwoViewSimulation tvs(CvMatUtil::initXs(tm.pnt, tm.nump), &cam1, &cam2);

		tvs.generateGroundTruth();

		cout<<"F: "<<CvMatUtil::matString(tvs.getF())<<endl;

		cout<<"validate F:"<<endl;
		tvs.validateF(tvs.getF());
	}
};

		}
	}
}