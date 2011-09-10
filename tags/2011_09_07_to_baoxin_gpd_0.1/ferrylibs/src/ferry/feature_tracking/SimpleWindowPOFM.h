#pragma once

#include "highgui.h"
#include "cv.h"

#include "PointsOutFeatureMatcher.h"
#include "SimpleDistanceBasedFM.h"
#include "SimpleWindowSSDFDC.h"


namespace ferry {
	namespace feature_tracking {

class SimpleWindowPOFM : public SimpleDistanceBasedFM, public PointsOutFeatureMatcher
{
public:
	SimpleWindowPOFM(SimpleWindowSSDFDC* pfdc, float maxDisparity = 50) : SimpleDistanceBasedFM(pfdc, maxDisparity){
		this->pswfdc = pfdc;
	}

	~SimpleWindowPOFM(void);

public:
	void compute(IplImage* im1, IplImage* im2, const vector<CvPoint>& corners1, const vector<CvPoint>& corners2, int* correspondences1, int* correspondences2) {
		computeDistanceMatrix(im1, im2, corners1, corners2);

		computeCorrespondences();
		
		getCorrespondences(correspondences1, correspondences2);
	}

	void compute(IplImage* im1, IplImage* im2, const vector<CvPoint>& corners1, const vector<CvPoint>& corners2, vector<CvPoint>& x1s, vector<CvPoint>& x2s) {
		//Timer timer;
		//timer.start();

		computeDistanceMatrix(im1, im2, corners1, corners2);

		//timer.end();
		//cout<<"calc distance matrix time: "<<timer.getSpan()<<endl;
		//timer.start();
		
		computeCorrespondences();

		//timer.end();
		//cout<<"calc correspondences time: "<<timer.getSpan()<<endl;

		//==
		double ls_errsum = 0;
		double o_errsum = 0;
		int num = 0;
		//==

		for (int i = 0; i < (int)corners1.size(); i++) {
			if (correspondences1[i] != -1) {
				x1s.push_back(corners1[i]);
				x2s.push_back(mincorners2[correspondences1[i]][i]);
				//== for comparison bewteen original window matching and local search augmented
				CvPoint p1 = corners1[i];
				CvPoint minp2 = mincorners2[correspondences1[i]][i];
				CvPoint p2 = corners2[correspondences1[i]];
				//cout<<"minp2: "<<minp2.x<<", "<<minp2.y<<" ........."<<endl;
				//cout<<"minssd: "<<pswfdc->computessd(im1, im2, p1, minp2)<<endl;
				//cout<<"distances: "<<distances[correspondences1[i]][i]<<endl;
				ls_errsum += distances[correspondences1[i]][i];
				//cout<<"p2: "<<p2.x<<", "<<p2.y<<" ..."<<endl;
				//cout<<"p2 ssd: "<<pswfdc->computessd(im1, im2, p1, p2)<<endl;
				o_errsum += pswfdc->computessd(im1, im2, p1, p2);
				num++;
				//==
			}
		}
		
		//===
		cout<<"/======================================"<<endl;
		cout<<"LS average : "<<(ls_errsum / num) <<endl;
		cout<<"O average : "<<o_errsum / num <<endl;
		//==
	}

	void computeDistanceMatrix(IplImage* im1, IplImage* im2, const vector<CvPoint>& corners1, const vector<CvPoint>& corners2) {
		count1 = (int)corners1.size();
		count2 = (int)corners2.size();
		for (int i = 0; i < count2; i++) {
			for (int j = 0; j < count1; j++) {
				if (calcDistance(corners1[j], corners2[i]) > maxDisparity) distances[i][j] = MAX_DISTANCE;
				else distances[i][j] = pswfdc->compute(im1, im2, corners1[j], corners2[i], mincorners2[i][j]);
			}
		}
	}

private:
	float calcDistance(const CvPoint& p1, const CvPoint& p2) {
		int dx = p1.x - p2.x;
		int dy = p1.y - p2.y;
		return sqrt((float)(dx * dx + dy * dy));
	}

private:
	SimpleWindowSSDFDC* pswfdc;

	CvPoint mincorners2[MAX_FEATURE_NUM][MAX_FEATURE_NUM];
};

	}
}