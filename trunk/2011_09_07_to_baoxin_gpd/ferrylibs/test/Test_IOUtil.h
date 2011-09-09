#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "cv.h"
#include <ferry/cv_geometry/IOUtil.h>

using namespace std;
using namespace ferry::cv_geometry::io_util;

namespace ferry {
	namespace cv_geometry {
		namespace test {

class Test_IOUtil
{
public:
	void test() {
		vector<CvPoint2D32f> pts;
		pts.push_back(cvPoint2D32f(2.1, 2.3));
		pts.push_back(cvPoint2D32f(4.1, 3.3));
		cout<<pts<<endl;

		char* file = "temp/points.txt";
		ofstream ofs(file);
		ofs<<pts<<endl;
		ofs.close();

		vector<CvPoint2D32f> ipts;
		ifstream ifs(file);
		ifs>>ipts;
		ifs.close();

		cout<<ipts<<endl;

		vector<CvPoint2D32f> ipts2;
		ifstream ifs2("temp/x1s.txt");
		ifs2>>ipts2;
		ifs2.close();

		cout<<ipts2<<endl;

	}
};

		}
	}
}