#pragma once

#include <iostream>
#include <ferry/cv_geometry/TwoViewGeometryUtil.h>
#include <ferry/cv_geometry/Camera.h>
#include "Test_MSVideo.h"

using namespace std;
using namespace ferry::cv_geometry;

namespace ferry {
	namespace cv_geometry {
		namespace test {

class Test_TwoViewGeometryUtil
{
public:
	void test() {
		Test_MSVideo tmsv;
		tmsv.readCalibrationFile("data/MS_Video/dancing/calibParams-breakdancers.txt");

		Camera* pcam1 = tmsv.cams[3];
		Camera* pcam2 = tmsv.cams[4];

		CvMat* F = TwoViewGeometryUtil::calcF(pcam1->getP(), pcam2->getP());

		cout<<"F: "<<CvMatUtil::matString(F)<<endl;
	}

	void testF() {

	}

};

		}
	}
}