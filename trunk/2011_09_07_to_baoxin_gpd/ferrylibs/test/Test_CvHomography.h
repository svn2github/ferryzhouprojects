#pragma once

#include <vector>

#include "highgui.h"
#include "cv.h"

#include <ferry/cv_geometry/Homography.h>
#include <ferry/cv_geometry/DLT_HC.h>
//#include <ferry/cv_geometry/HomographyCluster.h>
//#include "Test_GetCorrespondences.h"

using namespace std;
using namespace ferry::cv_geometry;

class Test_CvHomography
{
public:
	void test() {
		loadData();
		//int is[] = {77, 70, 78, 68};
		int is[] = {7, 38, 32, 36};
		vector<CvPoint2D32f> ix1s, ix2s;
		getIndexedPoints(is, ix1s, ix2s);
		DLT_HC dhc;
		Homography h = dhc.compute(ix1s, ix2s);

		for (int i = 0; i < x1s.size(); i++) {
			CvPoint2D32f p = h.transform(x1s[i]);
			cout<<i<<": "<<h.transfromDistance(x1s[i], x2s[i])<<" ("<<p.x<<", "<<p.y<<")"<<" <- ("<<x2s[i].x<<", "<<x2s[i].y<<")"<<endl;
		}
	}

	void getIndexedPoints(int* indices, vector<CvPoint2D32f>& ix1s, vector<CvPoint2D32f>& ix2s) {
		for (int i = 0; i < 4; i++) {
			ix1s.push_back(x1s[indices[i]]);
			ix2s.push_back(x2s[indices[i]]);
		}
	}

	void loadData() {
/*		Test_GetCorrespondences tgc;
		tgc.loadFeatureMatcher();
		//tgc.loadSimulatedData();
		pc = tgc.getCorrespondences();
		im1 = pc->im1;
		im2 = pc->im2;
		for (int i = 0; i < pc->x1s.size(); i++) {
			x1s.push_back(cvPointTo32f(pc->x1s[i]));
			x2s.push_back(cvPointTo32f(pc->x2s[i]));
		}
*/	}


private:
	vector<CvPoint2D32f> x1s;
	vector<CvPoint2D32f> x2s;
	IplImage* im1;
	IplImage* im2;
	//CvPointMatrix2DConvertInterface* p_pmci;

private:
//	Correspondences* pc;

};
