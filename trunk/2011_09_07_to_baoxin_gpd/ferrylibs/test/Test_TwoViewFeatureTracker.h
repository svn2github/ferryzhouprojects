#pragma once

#include <fstream>

#include <ferry/feature_tracking/TwoViewFeatureTracker.h>
#include <ferry/feature_tracking/BlockCornerDetector.h>

#include <ferry/cv_geometry/IOUtil.h>
#include <ferry/cv_geometry/DrawingUtil.h>

using namespace std;
using namespace ferry::cv_geometry;
using namespace ferry::cv_geometry::io_util;

namespace ferry {
	namespace feature_tracking {
		namespace test {

class Test_TwoViewFeatureTracker
{
public:
	void test() {
		CornerDetector* pcd = new OpenCVCornerDetector(400, 0.02, 8);
		//CornerDetector* pcd = new BlockCornerDetector(100, 10, 0.02, 15);
		PointsOutFeatureMatcher* ppofm = new SimpleWindowPOFM(new SimpleWindowSSDFDC(4, 3, 20), 200);
		//PointsOutFeatureMatcher* ppofm = new SimpleWindowPOFM(new SimpleWindowSSDFDC(5, 3, 30), 50);

		TwoViewFeatureTracker tvft(pcd, ppofm);

		char* file1 = "data/olympus/1_small.jpg";
		char* file2 = "data/olympus/3_small.jpg";
		//char* file1 = "data/olympus/im_134.bmp";
		//char* file2 = "data/olympus/im_136.bmp";

		IplImage* im1 = cvLoadImage(file1, 1);
		IplImage* im2 = cvLoadImage(file2, 1);

		vector<CvPoint> x1s, x2s;
		tvft.compute(im1, im2, x1s, x2s);

		draw_correspondences_image("cos1", im1, x1s, x2s);
		draw_correspondences_image("cos2", im1, im2, x1s, x2s);

		char* x1sfile = "temp/x1s.txt";
		char* x2sfile = "temp/x2s.txt";
		ofstream ofs1(x1sfile);
		ofs1<<x1s;
		ofs1.close();
		ofstream ofs2(x2sfile);
		ofs2<<x2s;
		ofs2.close();
	}
};

		}
	}
}